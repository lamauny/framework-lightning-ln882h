#include "wifi_manager.h"
#include "netif/ethernetif.h"
#include "dhcpd_api.h"
#include "ln_wifi_err.h"
#include "utils/debug/log.h"
#include "utils/ln_list.h"

#include "usr_app.h"

typedef struct {
    ln_list_t       list;
    uint8_t         node_count;
    uint32_t        update_ticks;
    sort_rule_t     rule;
    OS_Mutex_t      lock;
    int             enable;
} ap_info_list_ctrl_t;

static wifi_mgr_event_cb_t event_cb[WIFI_MGR_EVENT_MAX] = {NULL};
static ap_info_list_ctrl_t ap_info_list_ctrl = {0,};

static app_sta_scan_data_t *g_ap_data = NULL;

__STATIC_INLINE__ int ap_info_list_init(void)
{
    ap_info_list_ctrl_t *list_ctrl = &ap_info_list_ctrl;
    
    memset(list_ctrl, 0, sizeof(ap_info_list_ctrl_t));
    ln_list_init(&list_ctrl->list);
    list_ctrl->rule = RSSI_SORT;
    list_ctrl->enable = LN_TRUE;
    
    if (OS_OK != OS_MutexCreate(&list_ctrl->lock)) {
        return WIFI_ERR_OS_SERVICE;
    }
    return WIFI_ERR_NONE;
}

__STATIC_INLINE__ void ap_list_remove_life_timeout_node(ap_info_list_ctrl_t *list_ctrl)
{
    uint32_t earliest_ticks = (uint32_t)-1, curr_ticks = OS_GetTicks();
    ap_info_node_t *pnode, *tmp;
    
    LN_LIST_FOR_EACH_ENTRY_SAFE(pnode, tmp, ap_info_node_t, list, &list_ctrl->list) {
        if ((curr_ticks - pnode->life_ticks) >= (AP_INFO_LIFETIME*1000)) {
            ln_list_rm(&pnode->list);
            OS_Free(pnode);
            list_ctrl->node_count--;
        } else if(pnode->life_ticks < earliest_ticks) {
            earliest_ticks = pnode->life_ticks;
            list_ctrl->update_ticks = earliest_ticks;
        }
    }
    
    if (ln_is_list_empty(&list_ctrl->list)) {
        list_ctrl->update_ticks = 0;
    }
}


void wifi_manager_cleanup_scan_results(void)
{
    ap_info_list_ctrl_t *list_ctrl = &ap_info_list_ctrl;
    ap_info_node_t *pnode, *tmp;

    OS_MutexLock(&list_ctrl->lock, OS_WAIT_FOREVER);
    LN_LIST_FOR_EACH_ENTRY_SAFE(pnode, tmp, ap_info_node_t, list, &list_ctrl->list) {
        ln_list_rm(&pnode->list);
        OS_Free(pnode);
        list_ctrl->node_count--;
    }

    list_ctrl->update_ticks = 0;
    OS_MutexUnlock(&list_ctrl->lock);
}

int wifi_manager_ap_list_update_enable(int en)
{
    ap_info_list_ctrl_t * list_ctrl = &ap_info_list_ctrl;
    OS_MutexLock(&list_ctrl->lock, OS_WAIT_FOREVER);
    list_ctrl->enable = en;
    OS_MutexUnlock(&list_ctrl->lock);
    return WIFI_ERR_NONE;
}

__STATIC_INLINE__ int ap_info_list_update(const ap_info_t *info)
{
    ln_wifi_err_t ret = WIFI_ERR_NONE;
    uint32_t curr_ticks = 0;
    ap_info_list_ctrl_t * list_ctrl = &ap_info_list_ctrl;
    ap_info_node_t *pnode, *tmp, *node_new = NULL;

    OS_MutexLock(&list_ctrl->lock, OS_WAIT_FOREVER);
    if (list_ctrl->enable != LN_TRUE) {
        ret = WIFI_ERR_NONE;
        goto __exit;
    }

    // creat new info node.
    if (NULL == (node_new = OS_Malloc(sizeof(ap_info_node_t)))) {
        ret = WIFI_ERR_MEM;
        goto __exit;
    }
    memcpy(&node_new->info, info, sizeof(ap_info_t));
    curr_ticks = OS_GetTicks();
    node_new->life_ticks = curr_ticks;

    ap_list_remove_life_timeout_node(list_ctrl);

    if (ln_is_list_empty(&list_ctrl->list)) {
        ln_list_add(&list_ctrl->list, &node_new->list);
        list_ctrl->node_count++;
    } else {
        //list full,remove a node.
        if (list_ctrl->node_count >= AP_LIST_NODE_MAX) {
            pnode = LN_LIST_ENTRY(list_ctrl->list.prev, ap_info_node_t, list);
            ln_list_rm(list_ctrl->list.prev);
            OS_Free(pnode);
            list_ctrl->node_count--;
        }

        // lookup the same info node, romve it.
        LN_LIST_FOR_EACH_ENTRY_SAFE(pnode, tmp, ap_info_node_t, list, &list_ctrl->list) 
        {
            if (!memcmp(pnode->info.bssid, info->bssid, BSSID_LEN)) {
                ln_list_rm(&pnode->list);
                OS_Free(pnode);
                list_ctrl->node_count--;
            }
        }

        //add new node
        if (list_ctrl->rule == NORMAL_SORT) {
            ln_list_add(&list_ctrl->list, &node_new->list);
            list_ctrl->node_count++;
        } else { // RSSI_SORT
            /**
             * If the smaller rssi is not found after traversing the list, 
             * then the for loop exits finally,
             * now the "entry(&pnode->list)" points to the "head(&list_ctrl->list)".
             * At this time, needs to add item to the end of the list,
             * code as "if (&pnode->list == &list_ctrl->list)".
            */
            LN_LIST_FOR_EACH_ENTRY_SAFE(pnode, tmp, ap_info_node_t, list, &list_ctrl->list) {
                if (info->rssi >= pnode->info.rssi)
                {
                    ln_list_insert(&pnode->list, &node_new->list);
                    list_ctrl->node_count++;
                    break;
                }
            }

            if (&pnode->list == &list_ctrl->list)
            {
                ln_list_insert(&pnode->list, &node_new->list);
                list_ctrl->node_count++;
            }
        }
    }
    list_ctrl->update_ticks = curr_ticks;

__exit:
    OS_MutexUnlock(&list_ctrl->lock);
    return ret;
}

int wifi_manager_set_ap_list_sort_rule(sort_rule_t rule)
{
    ap_info_list_ctrl_t * list_ctrl = &ap_info_list_ctrl;
    ap_info_node_t *pnode, *tmp;
    
    OS_MutexLock(&list_ctrl->lock, OS_WAIT_FOREVER);
    if (list_ctrl->rule != rule) 
    {
        LN_LIST_FOR_EACH_ENTRY_SAFE(pnode, tmp, ap_info_node_t, list, &list_ctrl->list) {
            ln_list_rm(&pnode->list);
            OS_Free(pnode);
            list_ctrl->node_count--;
        }
        list_ctrl->rule = rule;
        list_ctrl->update_ticks = 0;
    }
    OS_MutexUnlock(&list_ctrl->lock);
    return WIFI_ERR_NONE;
}

int wifi_manager_get_ap_list(ln_list_t **list, uint8_t * node_count)
{
    if (!list || !node_count) {
        return WIFI_ERR_INVALID_PARAM;
    }
    
    OS_MutexLock(&ap_info_list_ctrl.lock, OS_WAIT_FOREVER);
    ap_info_list_ctrl_t * list_ctrl = &ap_info_list_ctrl;
    ap_list_remove_life_timeout_node(list_ctrl);
    *list = &list_ctrl->list;
    *node_count = list_ctrl->node_count;
    OS_MutexUnlock(&ap_info_list_ctrl.lock);
    
    return WIFI_ERR_NONE;
}

static void sta_startup(void)
{
    LOG(LOG_LVL_INFO, "STA startup!\r\n");
    if (event_cb[WIFI_MGR_EVENT_STA_STARTUP]) {
        event_cb[WIFI_MGR_EVENT_STA_STARTUP](NULL);
    }
}

static void sta_connected(void)
{
    LOG(LOG_LVL_INFO, "STA connected to target AP!\r\n");
    netdev_set_state(NETIF_IDX_STA, NETDEV_UP);

    if (event_cb[WIFI_MGR_EVENT_STA_CONNECTED]) {
        event_cb[WIFI_MGR_EVENT_STA_CONNECTED](NULL);
    }
    // Cleanup scan results
    wifi_manager_cleanup_scan_results();
}

static void sta_disconnected(void)
{
    LOG(LOG_LVL_INFO, "STA disconnected to target AP!\r\n");
    netdev_set_state(NETIF_IDX_STA, NETDEV_DOWN);
    wifi_set_allow_cpu_sleep_flag(0);

    if (event_cb[WIFI_MGR_EVENT_STA_DISCONNECTED]) {
        event_cb[WIFI_MGR_EVENT_STA_DISCONNECTED](NULL);
    }
}

//list init
static app_sta_scan_data_t *init_list(void)
{
    app_sta_scan_data_t *p = (app_sta_scan_data_t *)OS_Malloc(sizeof(app_sta_scan_data_t));
    p->next = NULL;
    return p;
}

//list delete
static void deinit_list(app_sta_scan_data_t *list)
{
    while(list != NULL)
    {
        OS_Free(list);
        list = list->next;
    }
    list = NULL;
}

//insert list
static app_sta_scan_data_t *insert_list(app_sta_scan_data_t *list, void *data)
{
    app_sta_scan_data_t *p = OS_Malloc(sizeof(app_sta_scan_data_t));

    memcpy(&p->data, (ap_info_t *)data, sizeof(ap_info_t));
    p->next = list->next;
    list->next = p;
    return list;
}

static void sta_scan_complete(void)
{
    ln_list_t *list;
    uint8_t node_count = 0;
    ap_info_node_t *pnode;
    app_sta_scan_cfg_t *cfg = &g_sta_scan_cfg;
    uint8_t channel_num = cfg->channel_num;
    if(g_ap_data != NULL && cfg->data_fetech == 0)
    {
        deinit_list(g_ap_data);
    }
    wifi_manager_ap_list_update_enable(LN_FALSE);
    wifi_manager_get_ap_list(&list, &node_count);
    // 2.print all ap info in the list.
    if(!cfg->data_fetech)
    {
        cfg->data_fetech = 1;
        g_ap_data = init_list();

        LN_LIST_FOR_EACH_ENTRY(pnode, ap_info_node_t, list,list)
        {
            uint8_t * mac = (uint8_t*)pnode->info.bssid;
            ap_info_t *ap_info = &pnode->info;
            switch(cfg->filter_mask)
            {
                //all list
                case 0x00:
                {
                    insert_list(g_ap_data, (void *)&pnode->info);
                }break;
                //channel
                case 0x01:
                {
                    for(uint8_t ch = 1; ch < cfg->channel_num; ch ++)
                    {
                        if(pnode->info.channel == cfg->channels[ch])
                            insert_list(g_ap_data, (void *)&pnode->info);
                    }
                }
                //bssid
                case 0x02:
                {
                    if(pnode->info.bssid[0] == cfg->bssid[1] && \
                       pnode->info.bssid[1] == cfg->bssid[2] && \
                       pnode->info.bssid[2] == cfg->bssid[3] && \
                       pnode->info.bssid[3] == cfg->bssid[4] && \
                       pnode->info.bssid[4] == cfg->bssid[5] && \
                       pnode->info.bssid[5] == cfg->bssid[6] \
                      )
                    {
                        insert_list(g_ap_data, (void *)&pnode->info);
                    }
                }break;
                //channel & bssid
                case 0x03:
                {
                    for(uint8_t ch = 1; ch < cfg->channel_num; ch ++)
                    {
                        if(pnode->info.channel == cfg->channels[ch])
                        {
                            if(pnode->info.bssid[0] == cfg->bssid[1] && \
                               pnode->info.bssid[1] == cfg->bssid[2] && \
                               pnode->info.bssid[2] == cfg->bssid[3] && \
                               pnode->info.bssid[3] == cfg->bssid[4] && \
                               pnode->info.bssid[4] == cfg->bssid[5] && \
                               pnode->info.bssid[5] == cfg->bssid[6] \
                              )
                            {
                                insert_list(g_ap_data, (void *)&pnode->info);
                            }
                            
                        }
                    }
                }break;
                //ssid
                case 0x04:
                {
                    if(!strcmp(cfg->ssid + 1, pnode->info.ssid))
                        insert_list(g_ap_data, (void *)&pnode->info);
                }break;
                //ssid & channel
                case 0x05:
                {
                    for(uint8_t ch = 1; ch < cfg->channel_num; ch ++)
                    {
                        if(pnode->info.channel == cfg->channels[ch])
                        {
                            if(!strcmp(cfg->ssid + 1, pnode->info.ssid))
                                insert_list(g_ap_data, (void *)&pnode->info);
                        }
                    }
                }break;
                //ssid & bssid
                case 0x06:
                {
                    if(pnode->info.bssid[0] == cfg->bssid[1] && \
                       pnode->info.bssid[1] == cfg->bssid[2] && \
                       pnode->info.bssid[2] == cfg->bssid[3] && \
                       pnode->info.bssid[3] == cfg->bssid[4] && \
                       pnode->info.bssid[4] == cfg->bssid[5] && \
                       pnode->info.bssid[5] == cfg->bssid[6] && (!strcmp(cfg->ssid + 1, pnode->info.ssid))\
                      )
                    {
                        insert_list(g_ap_data, (void *)&pnode->info);
                    }
                    
                }break;
                //ssid & bssid & channel
                case 0x07:
                {
                    for(uint8_t ch = 1; ch < cfg->channel_num; ch ++)
                    {
                        if(pnode->info.channel == cfg->channels[ch])
                        {
                            if(pnode->info.bssid[0] == cfg->bssid[1] && \
                               pnode->info.bssid[1] == cfg->bssid[2] && \
                               pnode->info.bssid[2] == cfg->bssid[3] && \
                               pnode->info.bssid[3] == cfg->bssid[4] && \
                               pnode->info.bssid[4] == cfg->bssid[5] && \
                               pnode->info.bssid[5] == cfg->bssid[6] && (!strcmp(cfg->ssid + 1, pnode->info.ssid))\
                              )
                            {
                                insert_list(g_ap_data, (void *)&pnode->info);
                            }
                        }
                    }
                }break;
                //default
                default:
                    break;
            }
        }
    }
    cfg->data = g_ap_data;

    wifi_manager_ap_list_update_enable(LN_TRUE);
    
    if (event_cb[WIFI_MGR_EVENT_STA_SCAN_COMPLETE]) {
        event_cb[WIFI_MGR_EVENT_STA_SCAN_COMPLETE](NULL);
    }
}

static void sta_scan_report(const ap_info_t *ap_info)
{
    ap_info_list_update(ap_info);
}

static void softap_startup(void)
{
    LOG(LOG_LVL_INFO, "SoftAP startup!\r\n");
    if (event_cb[WIFI_MGR_EVENT_SOFTAP_STARTUP]) {
        event_cb[WIFI_MGR_EVENT_SOFTAP_STARTUP](NULL);
    }
}

static void softap_associated(const uint8_t *mac_addr)
{
    uint8_t * mac = (uint8_t *)mac_addr;
    LOG(LOG_LVL_INFO, "[%02X:%02X:%02X:%02X:%02X:%02X] connected to softAP\r\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    //TODO: update sta list
    
    if (event_cb[WIFI_MGR_EVENT_SOFTAP_ASSOCIATED]) {
        event_cb[WIFI_MGR_EVENT_SOFTAP_ASSOCIATED]((void *)mac_addr);
    }
}

static void softap_disassociated(const uint8_t *mac_addr)
{
    uint8_t * mac = (uint8_t *)mac_addr;
    LOG(LOG_LVL_INFO, "[%02X:%02X:%02X:%02X:%02X:%02X] disconnected to softAP\r\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    //TODO: update sta list
   
    dhcpd_ip_release((char *)mac);

    if (event_cb[WIFI_MGR_EVENT_SOFTAP_DISASSOCIATED]) {
        event_cb[WIFI_MGR_EVENT_SOFTAP_DISASSOCIATED]((void *)mac_addr);
    }
}

static void wifi_callback_init(void)
{
    sta_cb_t sta_cb = {
        .startup       = &sta_startup,
        .connected     = &sta_connected,
        .disconnected  = &sta_disconnected,
        .scan_complete = &sta_scan_complete,
        .scan_report   = &sta_scan_report,
    };

    ap_cb_t  softap_cb = {
        .startup       = &softap_startup,
        .associated    = &softap_associated,
        .disassociated = &softap_disassociated,
    };
    
    memset(event_cb, 0, sizeof(event_cb));
    wifi_sta_reg_callback(&sta_cb);
    wifi_softap_reg_callback(&softap_cb);
}

int wifi_manager_init(void)
{
    ap_info_list_init();
    wifi_callback_init();
    return LN_TRUE;
}

int wifi_manager_deinit(void)
{
    //TODO: 
    return LN_TRUE;
}

int  wifi_manager_reg_event_callback(wifi_mgr_event_t event, wifi_mgr_event_cb_t cb)
{
    if (event >= WIFI_MGR_EVENT_MAX) {
        return WIFI_ERR_INVALID_PARAM;
    }
    
    event_cb[event] = cb;
    return WIFI_ERR_NONE;
}


