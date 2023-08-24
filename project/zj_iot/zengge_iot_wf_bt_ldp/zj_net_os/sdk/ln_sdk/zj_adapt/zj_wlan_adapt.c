
#include "zj_adapt_api.h"
#include "zj_adapt_config.h"
#include <stdbool.h>

#include "osal/osal.h"
#include "utils/debug/log.h"
#include "ln_at.h"
#include "utils/system_parameter.h"
#include "utils/sysparam_factory_setting.h"
#include "ln_utils.h"
#include "wifi.h"
#include "wifi_port.h"
#include "netif/ethernetif.h"
#include "wifi_manager.h"
#include "lwip/tcpip.h"
#include "drv_adc_measure.h"
#include "hal/hal_adc.h"
#include "ln_nvds.h"
#include "ln_wifi_err.h"
#include "ln_misc.h"
#include "ln882h.h"

#undef TAG
#undef ZJ_LOG
#define TAG  "WFAPT"
#define ZJ_LOCAL_LOG_LVL LOG_LVL_INFO

#define ZJ_LOG(fmt, ...)  LOG(ZJ_LOCAL_LOG_LVL, "["TAG"]"fmt, ##__VA_ARGS__)

#define PM_WIFI_DEFAULT_PS_MODE           (WIFI_NO_POWERSAVE)
#define WIFI_USER_USE_WPA3                (1) /* 0: Not use; 1: Use */

static uint32_t g_restart_time = 1000;
static uint8_t g_wifi_state = WIFI_STATE_IDLE;

static bool g_wifi_sta_stop_flag = false;
static bool g_is_wifi_initialized = false;
static uint8_t psk_value[40]      = {0x0};
static OS_Semaphore_t s_sem_scan = {.handle = NULL};

static void wifi_scan_complete_cb(void * arg)
{
    LN_UNUSED(arg);
    OS_SemaphoreRelease(&s_sem_scan);
    LOG(LOG_LVL_INFO, "adapt wifi scan complete\r\n");
}

static void wifi_connect_failed_cb(void * arg)
{
    wifi_sta_connect_failed_reason_t reason = *(wifi_sta_connect_failed_reason_t*)arg;
    LOG(LOG_LVL_INFO, "############ wifi_connect_failed!, reason = %d, please retry. ############\r\n", reason);
}

static void wifi_disconnect_cb(void *arg)
{
    LN_UNUSED(arg);
    g_wifi_state = WIFI_STATE_IDLE;
    // wifi_set_allow_cpu_sleep_flag(0);
}

static void wifi_connected_cb(void *arg)
{
    LN_UNUSED(arg);
    g_wifi_state = WIFI_STATE_CONNECTED_IP_GETTING;
}

static void tuya_wifi_get_ip_cb(struct netif *nif)
{
    LN_UNUSED(nif);
    g_wifi_state = WIFI_STATE_CONNECTED_IP_GOT;
}

static void ap_startup_cb(void *arg)
{
    LN_UNUSED(arg);
    netdev_set_state(NETIF_IDX_AP, NETDEV_UP);
    g_wifi_state = WIFI_STATE_WITH_AP_IDLE;
}

static void ap_assoc_ok_cb(void *arg)
{
    LN_UNUSED(arg);
    g_wifi_state = WIFI_STATE_WITH_AP_CONNECTED_IP_GOT;
}

static void ap_assoc_failed_cb(void *arg)
{
    LN_UNUSED(arg);
    g_wifi_state = WIFI_STATE_WITH_AP_IDLE;
}

static void ap_disassoc_cb(void *arg)
{
    LN_UNUSED(arg);
    g_wifi_state = WIFI_STATE_WITH_AP_DISCONNECT;
}

void zj_wifi_STA_Start(uint8_t *ssid,uint8_t ssid_len,uint8_t *pwd,uint8_t pwd_len)
{
    uint8_t mac_addr[6]        = {0x00, 0x50, 0xC2, 0x5E, 0xAA, 0xDA};
    wifi_sta_connect_t connect = {
        .ssid    = ssid,
        .pwd     = pwd,
        .bssid   = NULL,
        .psk_value = NULL,
    };

    wifi_scan_cfg_t scan_cfg = {
        .channel   = 0,
        .scan_type = WIFI_SCAN_TYPE_ACTIVE,
        .scan_time = 20,
    };

    ZJ_LOG("[%s:%d] ssid:%s, pwd:%s\r\n",
        __func__, __LINE__, (const char *)ssid, (const char *)pwd);

    if (!g_is_wifi_initialized) {
        zj_wifi_drv_init();
    }

    //1. sta mac get
     if (SYSPARAM_ERR_NONE != sysparam_sta_mac_get(mac_addr)) {
        LOG(LOG_LVL_ERROR, "[%s]sta mac get failed!!!\r\n", __func__);
        return;
    }

    if (mac_addr[0] == STA_MAC_ADDR0 &&
        mac_addr[1] == STA_MAC_ADDR1 &&
        mac_addr[2] == STA_MAC_ADDR2 &&
        mac_addr[3] == STA_MAC_ADDR3 &&
        mac_addr[4] == STA_MAC_ADDR4 &&
        mac_addr[5] == STA_MAC_ADDR5) {
        ln_generate_random_mac(mac_addr);
        sysparam_sta_mac_update((const uint8_t *)mac_addr);
    }

    // wifi_sta_disconnect();
    wifi_stop();
    netdev_set_state(NETIF_IDX_STA, NETDEV_DOWN);

    //1. net device(lwip)
    netdev_set_mac_addr(NETIF_IDX_STA, mac_addr);
    netdev_set_active(NETIF_IDX_STA);

    //2. wifi start
    wifi_manager_reg_event_callback(WIFI_MGR_EVENT_STA_SCAN_COMPLETE, &wifi_scan_complete_cb);
    wifi_manager_reg_event_callback(WIFI_MGR_EVENT_STA_CONNECT_FAILED, &wifi_connect_failed_cb);
    wifi_manager_reg_event_callback(WIFI_MGR_EVENT_STA_DISCONNECTED, wifi_disconnect_cb);
    wifi_manager_reg_event_callback(WIFI_MGR_EVENT_STA_CONNECTED, wifi_connected_cb);
    netdev_get_ip_cb_set(tuya_wifi_get_ip_cb);

#if (WIFI_USER_USE_WPA3 == 1)
    extern void ln_wpa_sae_enable(void);
    ln_wpa_sae_enable();
#endif

    if(WIFI_ERR_NONE != wifi_sta_start(mac_addr, PM_WIFI_DEFAULT_PS_MODE)){
        LOG(LOG_LVL_ERROR, "[%s]wifi sta start filed!!!\r\n", __func__);
    }

    connect.psk_value = NULL;
    if (strlen(connect.pwd) != 0) {
        if (0 == ln_psk_calc(connect.ssid, connect.pwd, psk_value, sizeof (psk_value))) {
            connect.psk_value = psk_value;
            hexdump(LOG_LVL_INFO, "psk value ", psk_value, sizeof(psk_value));
        }
    }

    wifi_set_11n_enable(0);
    wifi_sta_connect_v2(&connect, &scan_cfg, 30);
    g_wifi_state = WIFI_STATE_CONNECTING;

    LOG(LOG_LVL_INFO, "wifi sta connecting <%s>...\r\n", ssid);
}

void zj_wifi_STA_Stop()
{
    ZJ_LOG("[%s:%d] work mode:%d\r\n",
        __func__, __LINE__, wifi_current_mode_get());

    if (!g_is_wifi_initialized) {
        return;
    }

    wifi_sta_disconnect();
    wifi_stop();
    netdev_set_state(NETIF_IDX_STA, NETDEV_DOWN);

    g_wifi_state = WIFI_STATE_IDLE;
}

int wifi_mgmr_status_code_clean_internal()
{
    ZJ_LOG("[%s:%d]\r\n", __func__, __LINE__);
    return 0;
}


void zj_wifi_AP_Start(uint8_t *ssid,uint8_t ssid_len,uint8_t *pwd,uint8_t pwd_len)
{
    uint8_t mac_addr[6];
    wifi_softap_cfg_t ap_cfg = {0};
    uint8_t psk_value[40]    = {0};

    ZJ_LOG("[%s:%d] ssid:%s, pwd:%s\r\n",
        __func__, __LINE__, (const char *)ssid, (const char *)pwd);

    if (!g_is_wifi_initialized) {
        zj_wifi_drv_init();
    }

    //1. ap mac get
     if (SYSPARAM_ERR_NONE != sysparam_softap_mac_get(mac_addr)) {
        LOG(LOG_LVL_ERROR, "[%s]sta mac get failed!!!\r\n", __func__);
        return;
    }

    if (mac_addr[0] == SOFTAP_MAC_ADDR0 &&
        mac_addr[1] == SOFTAP_MAC_ADDR1 &&
        mac_addr[2] == SOFTAP_MAC_ADDR2 &&
        mac_addr[3] == SOFTAP_MAC_ADDR3 &&
        mac_addr[4] == SOFTAP_MAC_ADDR4 &&
        mac_addr[5] == SOFTAP_MAC_ADDR5) {
        ln_generate_random_mac(mac_addr);
        sysparam_softap_mac_update((const uint8_t *)mac_addr);
    }

    srand(PORT_CONFIG_SYSTEM_TIME);

    ap_cfg.ext_cfg.channel = rand() % 13 + 1;
    ap_cfg.ext_cfg.authmode = WIFI_AUTH_WPA_WPA2_PSK;
    ap_cfg.ext_cfg.ssid_hidden = 0;
    ap_cfg.ext_cfg.beacon_interval = 100;

    ap_cfg.ssid  = (char *)ssid;
    ap_cfg.pwd   = (char *)pwd;
    ap_cfg.bssid = (uint8_t *)mac_addr;

    LOG(LOG_LVL_INFO, "ssid:%s; pwd:%s; channel:%d, ecn:%d, max conn:%d, hidden:%d" \
        "beacon:%d, mac:%02x:%02x:%02x:%02x:%02x:%02x\r\n", 
        ssid, pwd, ap_cfg.ext_cfg.channel, 
        ap_cfg.ext_cfg.authmode, 3, ap_cfg.ext_cfg.ssid_hidden,
        ap_cfg.ext_cfg.beacon_interval, 
        mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);

    wifi_stop();
    netdev_set_state(NETIF_IDX_AP, NETDEV_DOWN);

    {
        tcpip_ip_info_t  ip_info = {0};
        server_config_t  server_config = {0};
        uint32_t dhcps_start = 0;
        uint32_t dhcps_end = 0;

        sysparam_softap_ip_info_get(&ip_info);
        sysparam_dhcpd_cfg_get(&server_config);

        // ip_info.ip.addr      = ipaddr_addr((const char *)cfg->ip.ip);
        // ip_info.gw.addr      = ipaddr_addr((const char *)cfg->ip.gw);
        // ip_info.netmask.addr = ipaddr_addr((const char *)cfg->ip.mask);

        // server_config.server.addr   = ip_info.ip.addr;
        // server_config.port          = 67;
        // server_config.lease         = 2880;
        // server_config.renew         = 2880;
        // server_config.client_max    = (cfg->max_conn > 3) ? 3 : cfg->max_conn;

        dhcps_start = (uint32_t)server_config.server.addr;
        dhcps_end   = (((dhcps_start >> 24) + 3) << 24) | (dhcps_start & 0x00FFFFFF);
        dhcps_start = (((dhcps_start >> 24) + 1) << 24) | (dhcps_start & 0x00FFFFFF);

        server_config.ip_start.addr = dhcps_start;
        server_config.ip_end.addr   = dhcps_end;

        if (LN_FALSE != dhcpd_is_running())
        {
            /* dhcpd is running, stop it first */
            dhcpd_stop();
        }

        if (DHCPD_ERR_NONE != dhcpd_curr_config_set(&server_config))
        {
            LOG(LOG_LVL_INFO, "dhcpd has been running!\r\n");
        }

        LOG(LOG_LVL_INFO, "ip info: \r\n");
        LOG(LOG_LVL_INFO, "\tip:      %s\r\n", ip4addr_ntoa(&ip_info.ip));
        LOG(LOG_LVL_INFO, "\tgw:      %s\r\n", ip4addr_ntoa(&ip_info.gw));
        LOG(LOG_LVL_INFO, "\tnetmask: %s\r\n", ip4addr_ntoa(&ip_info.netmask));

        LOG(LOG_LVL_INFO, "server config:\r\n");
        LOG(LOG_LVL_INFO, "\tip:       %s\r\n", ip4addr_ntoa(&server_config.server));
        LOG(LOG_LVL_INFO, "\tip start: %s\r\n", ip4addr_ntoa(&server_config.ip_start));
        LOG(LOG_LVL_INFO, "\tip end:   %s\r\n", ip4addr_ntoa(&server_config.ip_end));

        LOG(LOG_LVL_INFO,
                "port:%d\r\n" \
                "lease:%d\r\n" \
                "renew:%d\r\n" \
                "max client:%d\r\n",
                server_config.port,
                server_config.lease, server_config.renew, server_config.client_max);

        netdev_set_mac_addr(NETIF_IDX_AP, ap_cfg.bssid);
        netdev_set_ip_info(NETIF_IDX_AP, &ip_info);
        netdev_set_active(NETIF_IDX_AP);
    }

    ap_cfg.ext_cfg.psk_value = NULL;
    if ((ap_cfg.pwd != NULL && strlen((const char *)ap_cfg.pwd) > 0) &&
        (ap_cfg.ext_cfg.authmode != WIFI_AUTH_OPEN) &&
        (ap_cfg.ext_cfg.authmode != WIFI_AUTH_WEP)) {
        memset(psk_value, 0, sizeof(psk_value));
        if (0 == ln_psk_calc((const char *)ap_cfg.ssid, (const char *)ap_cfg.pwd, (uint8_t *)psk_value, sizeof(psk_value))) {
            ap_cfg.ext_cfg.psk_value = psk_value;
            hexdump(LOG_LVL_DEBUG, "psk value ", psk_value, sizeof(psk_value));
        }
    }
    wifi_manager_reg_event_callback(WIFI_MGR_EVENT_SOFTAP_STARTUP, &ap_startup_cb);
    wifi_manager_reg_event_callback(WIFI_MGR_EVENT_SOFTAP_ASSOCIATED, &ap_assoc_ok_cb);
    wifi_manager_reg_event_callback(WIFI_MGR_EVENT_SOFTAP_ASSOCIAT_FILED, &ap_assoc_failed_cb);
    wifi_manager_reg_event_callback(WIFI_MGR_EVENT_SOFTAP_DISASSOCIATED, &ap_disassoc_cb);

    if (0 != wifi_softap_start(&ap_cfg))
    {
        LOG(LOG_LVL_ERROR, "[%s:%d] wifi_softap_start() failed!", __func__, __LINE__);
    }

    wifi_softap_set_max_supp_sta_num(3);
    LOG(LOG_LVL_INFO, "wifi softap start, max supp <3> sta!\r\n");
}

void zj_wifi_AP_Stop()
{
    wifi_mode_t mode = wifi_current_mode_get();
    ZJ_LOG("[%s:%d] work mode:%d\r\n",
        __func__, __LINE__, mode);

    if (mode == WIFI_MODE_STATION) {
        return;
    }

    wifi_stop();
    netdev_set_state(NETIF_IDX_AP, NETDEV_DOWN);
}

static void reboot_timer_cb(void *arg)
{
    ln_chip_reboot();
}

void zj_restart_set_time(uint32_t time)
{
	g_restart_time = time;
}

/** 
 * @brief 1s后重启系统
 * @note  
 * @param[in] none
 * @return none
 */
int zj_restart_system(void)
{
    static TimerHandle_t g_reboot_timer;
    if(g_reboot_timer == NULL) {
        g_reboot_timer = xTimerCreate("rst_tmr", g_restart_time / portTICK_PERIOD_MS, 0,(void *)0,(void *)reboot_timer_cb);
        xTimerStart(g_reboot_timer,0);
    }
    return 0;
}

void zj_scan_router(zj_adapter_evt_t evt)
{
    ZJ_LOG("[%s:%d] evt:%d\r\n", __func__, __LINE__, evt);

    if (!g_is_wifi_initialized) {
        zj_wifi_drv_init();
    }

    if (WIFI_MODE_STATION != wifi_current_mode_get()) {
        return;
    }

    {
        #define CONNECTED_SCAN_TIMES     (6)
        #define DEFAULT_SCAN_TIMES       (1)
        #define SCAN_TIMEOUT             (1500)

        int8_t            scan_cnt   = DEFAULT_SCAN_TIMES;
        wifi_sta_status_t sta_status = WIFI_STA_STATUS_STARTUP;
        wifi_scan_cfg_t   scan_cfg   = {0,};
        sysparam_sta_scan_cfg_get(&scan_cfg);

        //1. creat sem, reg scan complete callback.
        if (OS_OK != OS_SemaphoreCreateBinary(&s_sem_scan))
        {
            return;
        }

        wifi_manager_reg_event_callback(WIFI_MGR_EVENT_STA_SCAN_COMPLETE, &wifi_scan_complete_cb);

        //2. start scan, wait scan complete
        wifi_get_sta_status(&sta_status);
        if (sta_status == WIFI_STA_STATUS_CONNECTED || \
            sta_status == WIFI_STA_STATUS_DISCONNECTING)
        {
            scan_cnt = CONNECTED_SCAN_TIMES;
        }
        LOG(LOG_LVL_INFO, "Scan cnt:%d; scan timeout:%d\r\n", scan_cnt, SCAN_TIMEOUT);

        for (; scan_cnt > 0; scan_cnt--)
        {
            LOG(LOG_LVL_INFO, "adapt wifi start sta scan\r\n");
            wifi_sta_scan(&scan_cfg);
            OS_SemaphoreWait(&s_sem_scan, OS_WAIT_FOREVER);
        }

        //3. scan complete,output ap list.
        {
            ln_list_t *list;
            uint8_t node_count = 0;
            ap_info_node_t *pnode;
            zj_wifi_scan_inf_t *wifi_scan_buff = NULL, *scan_ptr = NULL;

            wifi_manager_ap_list_update_enable(LN_FALSE);
            wifi_manager_get_ap_list(&list, &node_count);

            LOG(LOG_LVL_INFO, "AP cnt:%d\r\n", node_count);
            if (node_count <= 0) {
                LOG(LOG_LVL_INFO, "no scan result!\r\n");
                goto __finish;
            }

            // malloc memory to store ap list,and free in tuya_hal_wifi_release_ap()
            scan_ptr = wifi_scan_buff = OS_Malloc(node_count * sizeof(zj_wifi_scan_inf_t));
            if(NULL == wifi_scan_buff) {
                LOG(LOG_LVL_ERROR, "no memory!\r\n");
                goto __finish;
            }

            memset(wifi_scan_buff, 0, (sizeof(zj_wifi_scan_inf_t) * node_count));

            LN_LIST_FOR_EACH_ENTRY(pnode, ap_info_node_t, list, list)
            {
                ap_info_t *info = &pnode->info;

                scan_ptr->channel = info->channel;
                scan_ptr->rssi    = info->rssi;
                memcpy(scan_ptr->bssid, info->bssid, BSSID_LEN);
                memcpy(scan_ptr->ssid,  info->ssid,  33);
                scan_ptr->ssid[32] = '\0';

                scan_ptr->authmode = info->authmode;
                scan_ptr++;
                ZJ_LOG("ssid:%s, ch:%d, rssi:%d, auth:%d, "
                    "bssid:%02X:%02X:%02X:%02X:%02X:%02X\r\n",
                    info->ssid, info->channel, info->rssi, info->authmode,
                    info->bssid[0], info->bssid[1], info->bssid[2], info->bssid[3], info->bssid[4], info->bssid[5]);
            }

            // zj_adapter_post_event(evt, wifi_scan_buff, NULL, node_count);

__finish:
            //4. delete sem, callback
            wifi_manager_reg_event_callback(WIFI_MGR_EVENT_STA_SCAN_COMPLETE, NULL);
            if (wifi_scan_buff) {
                OS_Free(wifi_scan_buff);
            }
        }

        OS_SemaphoreDelete(&s_sem_scan);
        wifi_manager_ap_list_update_enable(LN_TRUE);
        wifi_manager_cleanup_scan_results();
        s_sem_scan.handle = NULL;
    }
    ZJ_LOG("scan finished!\r\n");
}


void zj_wifi_get_ip_info(char *ip_addr)
{
    tcpip_ip_info_t ip;

    ZJ_LOG("[%s:%d]\r\n", __func__, __LINE__);

    netdev_get_ip_info(netdev_get_active(), &ip);
    sprintf(ip_addr, "%s", ip4addr_ntoa((ip4_addr_t *)&ip.ip.addr));
}

void zj_wifi_get_gateway(char *ip_addr)
{
    tcpip_ip_info_t ip;

    ZJ_LOG("[%s:%d]\r\n", __func__, __LINE__);

    netdev_get_ip_info(netdev_get_active(), &ip);
    sprintf(ip_addr, "%s", ip4addr_ntoa((ip4_addr_t *)&ip.gw.addr));
}

ip_addr_t zj_wifi_get_LAN_broadcast_addr()
{
    tcpip_ip_info_t ip_info;

    ZJ_LOG("[%s:%d]\r\n", __func__, __LINE__);

    netdev_get_ip_info(netdev_get_active(), &ip_info);

    ip4_addr_t ip;
    ip.addr = ip_info.ip.addr; 
    ip.addr |= 0xFF000000; 
    return ip;
}

ip_addr_t zj_wifi_get_ip_address()
{
    ip4_addr_t ip = {.addr = 0};
    netif_idx_t net_if_idx;
    tcpip_ip_info_t ip_info;

    ZJ_LOG("[%s:%d]\r\n", __func__, __LINE__);

    if (wifi_current_mode_get() == WIFI_MODE_STATION) {
        net_if_idx = NETIF_IDX_STA;
    } else {
        net_if_idx = NETIF_IDX_AP;
    }

    if (NETDEV_LINK_UP != netdev_get_link_state(netdev_get_active())) {
        return ip;
    }

    if(!netif_is_link_up(netdev_get_netif(net_if_idx))){
        LOG(LOG_LVL_ERROR, "network is not up yet!\r\n");
        return ip;
    }

    netdev_get_ip_info(net_if_idx, &ip_info);

    ip.addr = ip_info.ip.addr;
    return ip;
}

void zj_wifi_set_ip_info(char *ip_addr)
{
    ZJ_LOG("[%s:%d] %s\r\n", __func__, __LINE__, ip_addr);
}

int zj_wifi_get_mac_info(char *mac_str)
{
    uint8_t mac_addr[6] = {0};
    wifi_mode_t mode = wifi_current_mode_get();
    if (mode == WIFI_MODE_STATION) {
        wifi_get_macaddr(STATION_IF, mac_addr);
    } else {
        wifi_get_macaddr(SOFT_AP_IF, mac_addr);
    }

    sprintf(mac_str, "%02X%02X%02X%02X%02X%02X",
        mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    return 0;
}

int zj_wifi_get_mac_hex(uint8_t *mac_hex)
{
    wifi_mode_t mode = wifi_current_mode_get();
    if (mode == WIFI_MODE_STATION) {
        wifi_get_macaddr(STATION_IF, mac_hex);
    } else {
        wifi_get_macaddr(SOFT_AP_IF, mac_hex);
    }
    return 0;
}

int zj_wifi_get_state()
{
    enum WIFI_STATE_ENUM_LIST state = WIFI_STATE_UNKNOWN;
    wifi_sta_status_t status = WIFI_STA_STATUS_STARTUP;
    wifi_get_sta_status(&status);
    ZJ_LOG("[%s:%d] wifi status:%d, gstatus:%d\r\n",
        __func__, __LINE__, status, g_wifi_state);
    // switch (status)
    // {
    // case WIFI_STA_STATUS_STARTUP:
    //     state = WIFI_STATE_IDLE;
    //     break;
    // case WIFI_STA_STATUS_SCANING:
    //     state = WIFI_STATE_IDLE;
    //     break;
    // case WIFI_STA_STATUS_CONNECTING:
    //     state = WIFI_STATE_CONNECTING;
    //     break;
    // case WIFI_STA_STATUS_CONNECTED:
    //     state = WIFI_STATE_CONNECTED_IP_GETTING;
    //     break;
    // case WIFI_STA_STATUS_DISCONNECTING:
    // case WIFI_STA_STATUS_DISCONNECTED:
    //     state = WIFI_STATE_DISCONNECT;
    //     break;
    // default:
    //     break;
    // }
    //int     wifi_get_sta_conn_fail_reason(wifi_sta_connect_failed_reason_t *reason);
    return g_wifi_state;
}

int zj_get_ap_rssi()
{
    int8_t rssi = 0;
    wifi_sta_get_rssi(&rssi);
    return (int)rssi;
}

uint8_t zj_get_lan_code()
{
    ZJ_LOG("[%s:%d]\r\n", __func__, __LINE__);
    if(g_wifi_state == WIFI_STATE_CONNECTED_IP_GOT) {
        return 0;
    } else {
        return g_wifi_state;
    }
}

static void wifi_temp_calibrate_timer_cb(void *arg)
{
    uint16_t adc_val = 0;
    int16_t curr_adc = 0;
    volatile uint32_t *cap_reg_val = (uint32_t *)0x40100014;
    volatile uint32_t *awo_sys_pll_reg_val = (uint32_t *)0x40100074;
    LN_UNUSED(arg);

    extern uint16_t rf_read_ln882x(uint8_t page_id, uint8_t addr);

    adc_val = drv_adc_read(ADC_CH0);
    wifi_do_temp_cal_period(adc_val);

    curr_adc = (adc_val & 0xFFF);
    static volatile uint8_t cnt = 0;
    extern int8_t get_rssi(void);
    cnt++;
    if ((cnt % 10) == 0) {
        LOG(LOG_LVL_INFO,
                "adc raw: %4d, temp_IC: %d, cap:0x%08X, rf43:0x%04X"
                ", reg74:0x%04X, rssi:%d\r\n",
                curr_adc, (int16_t)(25 + (curr_adc - 770) / 2.54f),
                (uint32_t)(*cap_reg_val), rf_read_ln882x(0, 0x43),
                (uint32_t)(*awo_sys_pll_reg_val),
                (int8_t)get_rssi());
    }
}

void zj_wifi_drv_init()
{
    ZJ_LOG("[%s:%d]\r\n", __func__, __LINE__);

    if (g_is_wifi_initialized) {
        return;
    }

    //7.rf preprocess,img cal
    wifi_rf_calibration();

    //8.Init wifi stack.
    wifi_init();
    wlib_pvtcmd_output_cb_set(ln_at_vprintf);
    wifi_manager_init();

    // Start wifi temperature calibrate
    {
        int8_t cap_comp = 0;
        static OS_Timer_t s_wifi_temp_cal_timer = {0};

        if (NVDS_ERR_OK == ln_nvds_get_xtal_comp_val((uint8_t *)&cap_comp)) {
            if ((uint8_t)cap_comp == 0xFF) {
                cap_comp = 0;
            }
        }

        drv_adc_init();
        wifi_temp_cal_init(drv_adc_read(ADC_CH0), cap_comp);

        if (OS_OK != OS_TimerCreate(&s_wifi_temp_cal_timer, OS_TIMER_PERIODIC,
                wifi_temp_calibrate_timer_cb, NULL, 1000)) {
            LOG(LOG_LVL_ERROR, "create wifi temp calibrate timer failed.\r\n");
        } else {
            OS_TimerStart(&s_wifi_temp_cal_timer);
        }
    }
    ZJ_LOG("LN wifi init ok, calibration pass!\r\n");
    zj_adapter_post_event(ADAPT_EVT_WIFI_INIT_DONE,NULL,NULL,0);

    g_is_wifi_initialized = true;
}
