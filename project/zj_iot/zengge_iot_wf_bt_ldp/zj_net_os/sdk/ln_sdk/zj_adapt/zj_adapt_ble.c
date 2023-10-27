
#include "zj_adapt_api.h"
#include "zj_adapt_config.h"
#include <stdbool.h>
/* BLE */

#include "osal/osal.h"
#include "utils/debug/ln_assert.h"
#include "utils/debug/log.h"
#include "ln_utils.h"
#include "ln_misc.h"
#include "gapm_task.h"
#include "gapc_task.h"
#include "gattc_task.h"
#include "gattm_task.h"
#include "gap.h"

#include "ln_ble_gap.h"
#include "ln_ble_gatt.h"
#include "ln_ble_advertising.h"
#include "ln_ble_scan.h"
#include "ln_ble_rw_app_task.h"
#include "ln_ble_app_kv.h"
#include "ln_ble_connection_manager.h"
#include "ln_ble_event_manager.h"
#include "ln_ble_smp.h"
#include "ln_ble_app_default_cfg.h"

enum trans_svc_att_idx
{
    DATA_TRANS_DECL_SVC = 0,
    DATA_TRANS_DECL_CHAR_RX,
    DATA_TRANS_DECL_CHAR_RX_VAL,
    DATA_TRANS_DECL_CHAR_TX,
    DATA_TRANS_DECL_CHAR_TX_VAL,
    DATA_TRANS_DECL_CHAR_TX_CCC,

    DATA_TRANS_IDX_MAX,
};

enum {
    DEVICE_ADV,
    BTR_FORWARD_ADV,
};

enum {
    BLE_STATE_UNINITIALIZED = 0,
    BLE_STATE_DISABLE,
    BLE_STATE_ENABLE,
};
#define CHAR_VAL_MAX_LEN    1024

#define ADV_SCAN_UNIT(_ms) ((_ms) * 8 / 5)
#define SCAN_INTERVAL_MS 70
#define SCAN_WINDOW_MS   20
#define SCAN_INTERVAL    ADV_SCAN_UNIT(SCAN_INTERVAL_MS)
#define SCAN_WINDOW      ADV_SCAN_UNIT(SCAN_WINDOW_MS)




#if (BLE_DATA_TRANS_SERVER)
struct user_svc_desc {
    ln_trans_svr_desc_t desc;
    uint8_t ccc;//character client config
};
#endif

#if (BLE_DATA_TRANS_CLIENT)
struct user_cli_desc {
    uint8_t con_idx;
    uint16_t start_handle;
    uint16_t end_handle;
    uint8_t svc_uuid_len;
    uint8_t svc_uuid[16];
};
#endif

typedef struct
{
    uint16_t  id;
    uint16_t  len;
    void     *msg;
} ble_usr_msg_t;

#define ZJ_BLE_TASK_STACK_SIZE      (1024)
#define DATA_TRANS_SVR_MAX          (2)

#define DATA_TRANS_1ST_SVR_UUID     {0xff, 0xff}
#define DATA_TRANS_1ST_RX_UUID      {0x01, 0xff}
#define DATA_TRANS_1ST_TX_UUID      {0x02, 0xff}

#define DATA_TRANS_2ND_SVR_UUID     {0x00, 0xfe}
#define DATA_TRANS_2ND_RX_UUID      {0x11, 0xff}
#define DATA_TRANS_2ND_TX_UUID      {0x22, 0xff}

uint16_t g_zj_mtu = 0;
static int g_ble_init_flag = BLE_STATE_UNINITIALIZED;
static OS_Thread_t g_zj_ble_app_thread;
static uint8_t g_btr_forward_p[26];

#if (BLE_DATA_TRANS_SERVER)
static const ln_attm_desc_t data_trans_1st_atts_db[] = {
    [DATA_TRANS_DECL_SVC] = {
        .uuid = {0x00, 0x28},
        .perm = PERM_MASK_RD,
        .ext_perm = (0 << PERM_POS_UUID_LEN),
        .max_size = 0,
    },
    [DATA_TRANS_DECL_CHAR_RX] = {
        .uuid = {0x03, 0x28},
        .perm = PERM_MASK_RD,
        .ext_perm = 0,
        .max_size = 0,
    },
    [DATA_TRANS_DECL_CHAR_RX_VAL] = {
        .uuid = DATA_TRANS_1ST_RX_UUID,
        .perm = PERM_MASK_WRITE_REQ | PERM_MASK_WRITE_COMMAND | PERM_MASK_RD,
        .ext_perm = (1 << PERM_POS_RI),
        .max_size = CHAR_VAL_MAX_LEN,
    },
    [DATA_TRANS_DECL_CHAR_TX] = {
        .uuid       = {0x03, 0x28},
        .perm       = PERM_MASK_RD,
        .ext_perm   = 0,
        .max_size   = 0,
    },
    [DATA_TRANS_DECL_CHAR_TX_VAL] = {
        .uuid       = DATA_TRANS_1ST_TX_UUID,
        .perm       = PERM_MASK_NTF | PERM_MASK_RD,
        .ext_perm   = (1 << PERM_POS_RI),
        .max_size   = CHAR_VAL_MAX_LEN,
    },
    [DATA_TRANS_DECL_CHAR_TX_CCC] = {
        .uuid       = {0x02, 0x29},
        .perm       = PERM_MASK_WRITE_REQ | PERM_MASK_RD,
        .ext_perm    = 0,
        .max_size    = 0,
    },
};

static const ln_attm_desc_t data_trans_2nd_atts_db[] = {
    [DATA_TRANS_DECL_SVC] = {
        .uuid = {0x00, 0x28},
        .perm = PERM_MASK_RD,
        .ext_perm = (0 << PERM_POS_UUID_LEN),
        .max_size = 0,
    },
    [DATA_TRANS_DECL_CHAR_RX] = {
        .uuid = {0x03, 0x28},
        .perm = PERM_MASK_RD,
        .ext_perm = 0,
        .max_size = 0,
    },
    [DATA_TRANS_DECL_CHAR_RX_VAL] = {
        .uuid = DATA_TRANS_2ND_RX_UUID,
        .perm = PERM_MASK_WRITE_REQ | PERM_MASK_WRITE_COMMAND | PERM_MASK_RD,
        .ext_perm = (1 << PERM_POS_RI),
        .max_size = CHAR_VAL_MAX_LEN,
    },
    [DATA_TRANS_DECL_CHAR_TX] = {
        .uuid       = {0x03, 0x28},
        .perm       = PERM_MASK_RD,
        .ext_perm   = 0,
        .max_size   = 0,
    },
    [DATA_TRANS_DECL_CHAR_TX_VAL] = {
        .uuid       = DATA_TRANS_2ND_TX_UUID,
        .perm       = PERM_MASK_NTF | PERM_MASK_RD,
        .ext_perm   = (1 << PERM_POS_RI),
        .max_size   = CHAR_VAL_MAX_LEN,
    },
    [DATA_TRANS_DECL_CHAR_TX_CCC] = {
        .uuid       = {0x02, 0x29},
        .perm       = PERM_MASK_WRITE_REQ | PERM_MASK_RD,
        .ext_perm    = 0,
        .max_size    = 0,
    },
};

static struct user_svc_desc g_user_svc_desc_tab[DATA_TRANS_SVR_MAX] = {
    {
        .desc = {
            .start_handle   = LN_ATT_INVALID_HANDLE,
            .svr_uuid_len   = 2,
            .svr_uuid       = DATA_TRANS_1ST_SVR_UUID,
            .att_count      = sizeof(data_trans_1st_atts_db)/sizeof(data_trans_1st_atts_db[0]),
            .att_desc       = &data_trans_1st_atts_db,
        },
        .ccc = 0,
    },
    {
        .desc = {
            .start_handle   = LN_ATT_INVALID_HANDLE,
            .svr_uuid_len   = 2,
            .svr_uuid       = DATA_TRANS_2ND_SVR_UUID,
            .att_count      = sizeof(data_trans_2nd_atts_db)/sizeof(data_trans_2nd_atts_db[0]),
            .att_desc        = &data_trans_2nd_atts_db,
        },
        .ccc = 0
    }
};
#endif

#if (BLE_DATA_TRANS_CLIENT)
struct user_cli_desc g_user_cli_desc_tab[DATA_TRANS_SVR_MAX] = {
    {
        .con_idx        = APP_CONN_INVALID_IDX,
        .start_handle   = LN_ATT_INVALID_HANDLE,
        .end_handle     = LN_ATT_INVALID_HANDLE,
        .svc_uuid_len   = 16,
        .svc_uuid       = DATA_TRANS_1ST_SVR_UUID,
    },
    {
        .con_idx        = APP_CONN_INVALID_IDX,
        .start_handle   = LN_ATT_INVALID_HANDLE,
        .end_handle     = LN_ATT_INVALID_HANDLE,
        .svc_uuid_len   = 2,
        .svc_uuid       = DATA_TRANS_2ND_SVR_UUID,
    },
};
#endif

static void app_set_adv_data(void)
{
    //adv data: adv length--adv type--adv string ASCII
    uint8_t adv_data_len = 0;
    uint8_t adv_data[ADV_DATA_LEGACY_MAX] = {0};
    ln_adv_data_t adv_data_param;

    uint8_t *cus_adv_data = zj_adapter_get_adv_data_config();

#if (BLE_NET_VERSION == 5)
    adv_data[adv_data_len++] = 21;
    adv_data[adv_data_len++] = GAP_AD_TYPE_COMPLETE_NAME;
    memcpy(adv_data+2, cus_adv_data+5, 20);
    adv_data_len += 20;
#else
    adv_data[adv_data_len++] = 17;
    adv_data[adv_data_len++] = GAP_AD_TYPE_SERVICE_16_BIT_DATA;
    memcpy(adv_data+adv_data_len, cus_adv_data+5, 16);
    adv_data_len += 16;
    adv_data[adv_data_len++] = 9;
    adv_data[adv_data_len++] = GAP_AD_TYPE_COMPLETE_NAME;
    memcpy(adv_data+adv_data_len, cus_adv_data+23, 8);
    adv_data_len += 8;
#endif

    adv_data_param.length = adv_data_len;
    adv_data_param.data = adv_data;
    ln_ble_adv_data_set(&adv_data_param);

    hexdump(LOG_LVL_INFO, (const char *)"legacy_adv", adv_data, adv_data_len);
}

static void app_set_scan_resp_data(void)
{
    //adv data: adv length--adv type--adv string ASCII
    uint8_t adv_data_len = 0;
    uint8_t adv_data[31] = {0};
    ln_adv_data_t adv_data_param;

    uint8_t *cus_adv_rsp_data = zj_adapter_get_adv_scanResp_config();

    adv_data[adv_data_len++] = 30;
    adv_data[adv_data_len++] = 0xff;
    memcpy(adv_data+adv_data_len, cus_adv_rsp_data+2, 29);
    adv_data_len += 29;

    adv_data_param.length = adv_data_len;
    adv_data_param.data = adv_data;
    ln_ble_adv_scan_rsp_data_set(&adv_data_param);

    hexdump(LOG_LVL_INFO, (const char *)"legacy_scan_rsp", adv_data, adv_data_len);
}

static void ln_ble_connect_cb(void *arg)
{
    ble_evt_connected_t * evt_conn = (ble_evt_connected_t *)arg;

    uint8_t conn_idx = evt_conn->conn_idx;
    LOG(LOG_LVL_TRACE, "ln_ble_connect conn_id=%d\r\n", conn_idx);

    zj_adapter_post_event(ADAPT_EVT_BLE_CONNECTED,NULL,NULL,0);
}

static void ln_ble_disconnect_cb(void *arg)
{
    ble_evt_disconnected_t *evt_disconn = (ble_evt_disconnected_t *)arg;
    uint8_t ble_role = ln_kv_ble_usr_data_get()->ble_role;
    LOG(LOG_LVL_TRACE, "ln_ble_disconnect conn_id=%d\r\n", evt_disconn->conn_idx);

    zj_adapter_post_event(ADAPT_EVT_BLE_DISCONNECTED,NULL,NULL,0);

    if((ble_role & BLE_ROLE_PERIPHERAL)) {
        //ln_ble_adv_start();

#if (BLE_DATA_TRANS_SERVER)
        for(int i=0;i<DATA_TRANS_SVR_MAX;i++)
            g_user_svc_desc_tab[i].ccc = 0;
#endif
    }

    if((ble_role & BLE_ROLE_CENTRAL)) {
#if (BLE_DATA_TRANS_CLIENT)
        for(int i=0;i<DATA_TRANS_SVR_MAX;i++) {
            g_user_cli_desc_tab[i].con_idx        = APP_CONN_INVALID_IDX;
            g_user_cli_desc_tab[i].start_handle   = LN_ATT_INVALID_HANDLE;
            g_user_cli_desc_tab[i].end_handle     = LN_ATT_INVALID_HANDLE;
        }
#endif
    }
}

static void ln_ble_scan_report_cb(void *arg)
{
    ble_evt_scan_report_t *p_scan_rpt = (ble_evt_scan_report_t *)arg;

    LOG(LOG_LVL_TRACE, "\r\n--------------adv_report-------------\r\n");
    LOG(LOG_LVL_TRACE, "rssi=%d, tx_pwr=%d\r\n", p_scan_rpt->rssi, p_scan_rpt->tx_pwr);
    LOG(LOG_LVL_TRACE, "addr_type=%d, addr=0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x\r\n",
            p_scan_rpt->trans_addr_type, p_scan_rpt->trans_addr[0], p_scan_rpt->trans_addr[1],
            p_scan_rpt->trans_addr[2], p_scan_rpt->trans_addr[3], p_scan_rpt->trans_addr[4], p_scan_rpt->trans_addr[5]);
    //hexdump(LOG_LVL_INFO, "adv data: ", (void *)p_scan_rpt->data, p_scan_rpt->length);
    uint8_t adv_data[40];
    memcpy(adv_data, p_scan_rpt->data, p_scan_rpt->length);
    memcpy(adv_data+p_scan_rpt->length, p_scan_rpt->trans_addr, 6);
    zj_adapter_post_event(ADAPT_EVT_BLE_RMT, adv_data, (int *)&(p_scan_rpt->rssi), p_scan_rpt->length + 6);

}

#if (BLE_DATA_TRANS_SERVER)
static void ln_ble_gatt_write_req_cb(void *arg)
{
    ble_evt_gatt_write_req_t *p_gatt_write = (ble_evt_gatt_write_req_t *)arg;
    LOG(LOG_LVL_TRACE, "ln_ble_gatt_write conn_id=%d,handle=%d\r\n",p_gatt_write->conidx,p_gatt_write->handle);
    hexdump(LOG_LVL_INFO, "[recv data]", (void *)p_gatt_write->value, p_gatt_write->length);

    if(p_gatt_write->handle == g_user_svc_desc_tab[0].desc.start_handle +DATA_TRANS_DECL_CHAR_TX_CCC)
        g_user_svc_desc_tab[0].ccc = *((uint16_t *)p_gatt_write->value);

    if(p_gatt_write->handle == g_user_svc_desc_tab[1].desc.start_handle +DATA_TRANS_DECL_CHAR_TX_CCC)
        g_user_svc_desc_tab[1].ccc = *((uint16_t *)p_gatt_write->value);

    if(p_gatt_write->handle == g_user_svc_desc_tab[0].desc.start_handle +DATA_TRANS_DECL_CHAR_RX_VAL)
        zj_adapter_post_event(ADAPT_EVT_BLE_LIGHT_DATA,p_gatt_write->value,NULL,p_gatt_write->length);

    if(p_gatt_write->handle == g_user_svc_desc_tab[1].desc.start_handle +DATA_TRANS_DECL_CHAR_RX_VAL)
        zj_adapter_post_event(ADAPT_EVT_BLE_OTA_DATA,p_gatt_write->value,NULL,p_gatt_write->length);
}
#endif

static void ln_ble_stack_init(void)
{
    //1.controller init
    ln_bd_addr_t bt_addr = {0};
#ifdef BLE_USE_STATIC_PUBLIC_ADDR
    ln_bd_addr_t *kv_addr = ln_kv_ble_pub_addr_get();
    memcpy(&bt_addr, kv_addr, sizeof(ln_bd_addr_t));
#else
    ln_generate_random_mac(bt_addr.addr);
    bt_addr.addr[5] |= 0xC0;
#endif
    extern void rw_init(uint8_t mac[6]);
    rw_init(bt_addr.addr);

    //2.host init
    ln_gap_app_init();
    ln_gatt_app_init();

    //3.app component init
    ln_ble_conn_mgr_init();
    ln_ble_evt_mgr_init();
    ln_ble_smp_init();

    uint8_t role = ln_kv_ble_usr_data_get()->ble_role;

    if(role & BLE_ROLE_PERIPHERAL) {
        ln_ble_adv_mgr_init();
    }

    if((role & BLE_ROLE_CENTRAL)) {
        ln_ble_scan_mgr_init();
    }

#if (BLE_DATA_TRANS_SERVER)
    ln_ble_trans_svr_init();
#endif
#if (BLE_DATA_TRANS_CLIENT)
    ln_ble_trans_cli_init();
#endif

    ln_rw_app_task_init();

    //4.stack start
    ln_gap_reset();
    
    uint8_t *mac = bt_addr.addr;
    LOG(LOG_LVL_INFO, "+--------------- ble stack init ok -----------+\r\n");
    LOG(LOG_LVL_INFO, "|ble role : %-22d            |\r\n",  role);
    LOG(LOG_LVL_INFO, "|ble mac  : [%02X:%02X:%02X:%02X:%02X:%02X] %-13s |\r\n", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0], "");
    LOG(LOG_LVL_INFO, "+---------------------------------------------+\r\n");
}

static void ln_ble_app_init(void)
{
    uint8_t role = ln_kv_ble_usr_data_get()->ble_role;
    LOG(LOG_LVL_TRACE, "ble_app_init role=%d\r\n", role);

    ln_ble_evt_mgr_reg_evt(BLE_EVT_ID_CONNECTED,    ln_ble_connect_cb);
    ln_ble_evt_mgr_reg_evt(BLE_EVT_ID_DISCONNECTED, ln_ble_disconnect_cb);

    if(role & BLE_ROLE_PERIPHERAL)
    {
#if (BLE_DATA_TRANS_SERVER)
        for(int i = 0;i<DATA_TRANS_SVR_MAX;i++) {
            ln_ble_trans_svr_add(&g_user_svc_desc_tab[i].desc);
        }
        ln_ble_evt_mgr_reg_evt(BLE_EVT_ID_GATT_WRITE_REQ, ln_ble_gatt_write_req_cb);
#endif
        /*advertising activity init*/
        adv_param_t *adv_param = &le_adv_mgr_info_get()->adv_param;
        adv_param->adv_type = GAPM_ADV_TYPE_LEGACY;
        adv_param->adv_prop = GAPM_ADV_PROP_UNDIR_CONN_MASK;
        ln_ble_adv_actv_creat(adv_param);
        /*set advertising data*/
        app_set_adv_data();
        /*set scan respond data*/
        app_set_scan_resp_data();

        /*start advertising*/
        //ln_ble_adv_start();
    }

    if((role & BLE_ROLE_CENTRAL))
    {
        ln_ble_evt_mgr_reg_evt(BLE_EVT_ID_SCAN_REPORT, ln_ble_scan_report_cb);

        /*scan activity init*/
        ln_ble_scan_actv_creat();
        /*start scan*/
        //ln_ble_scan_start(&le_scan_mgr_info_get()->scan_param);
        /*connect activity init*/
        //ln_ble_init_actv_creat();
    }
}

void bleprph_host_task(void *param)
{

}

void zj_ble_scan_stop()
{
    if(BLE_STATE_UNINITIALIZED == g_ble_init_flag)
        return ;

    if(LE_SCAN_STATE_STARTING == le_scan_state_get() 
            || LE_SCAN_STATE_STARTED == le_scan_state_get()) {
        ln_ble_scan_stop();
    }
}

void zj_ble_scan_start()
{
    if(BLE_STATE_ENABLE != g_ble_init_flag)
        return ;

    if(LE_SCAN_STATE_INITIALIZED == le_adv_state_get()
            ||LE_SCAN_STATE_STOPING == le_scan_state_get() 
            || LE_SCAN_STATE_STOPED == le_scan_state_get()) {
        ln_ble_scan_start(&le_scan_mgr_info_get()->scan_param);
    }
}

void zj_btr_adv_payload_update(uint8_t *p)
{

}

void zj_ble_adv_update()
{
    if(zj_adapter_post_adv_update_condition_get()){
        ln_ble_adv_stop();
        OS_MsDelay(600);
        ln_ble_adv_start();
    }
}

uint8_t zj_ble_get_connected_status()
{
    LOG(LOG_LVL_INFO, "zj_ble_get_connected_status %d\r\n", ln_ble_is_connected());
    return ln_ble_is_connected();
}

void zj_ble_notify(uint8_t *data, int len)
{
    ln_trans_svr_send_t data_send;
    LOG(LOG_LVL_INFO, "zj_ble_notify\r\n");
    if(g_user_svc_desc_tab[0].ccc && LN_ATT_INVALID_HANDLE != g_user_svc_desc_tab[0].desc.start_handle) {
        data_send.conn_idx = 0;
        data_send.hdl = g_user_svc_desc_tab[0].desc.start_handle +DATA_TRANS_DECL_CHAR_TX_VAL; 
        data_send.len = len;
        data_send.data = data;

        ln_ble_trans_svr_ntf(&data_send);
    }
}

void zj_ble_ota_notify(uint8_t *data, int len)
{
    ln_trans_svr_send_t data_send;

    if(g_user_svc_desc_tab[1].ccc && LN_ATT_INVALID_HANDLE != g_user_svc_desc_tab[0].desc.start_handle) {
        data_send.conn_idx = 0;
        data_send.hdl = g_user_svc_desc_tab[1].desc.start_handle +DATA_TRANS_DECL_CHAR_TX_VAL; 
        data_send.len = len;
        data_send.data = data;

        ln_ble_trans_svr_ntf(&data_send);
    }
}

uint16_t zj_ble_get_mtu()
{
    if(g_zj_mtu > 255){

        g_zj_mtu = 255;
    }
    LOG(LOG_LVL_INFO, "zj_ble_get_mtu %d\r\n", g_zj_mtu);
    return g_zj_mtu;
}

void zj_ble_adv_start()
{
    if(BLE_STATE_ENABLE != g_ble_init_flag)
        return ;

    if(LE_ADV_STATE_INITIALIZED == le_adv_state_get() 
            || LE_ADV_STATE_STOPING == le_adv_state_get() 
            || LE_ADV_STATE_STOPED == le_adv_state_get()) {
        ln_ble_adv_start();
    }
}

void zj_ble_adv_stop()
{
    if(BLE_STATE_UNINITIALIZED == g_ble_init_flag)
        return ;

    if(LE_ADV_STATE_STARTING == le_adv_state_get() 
            || LE_ADV_STATE_STARTED == le_adv_state_get()) {
        ln_ble_adv_stop();
    }
}

void zj_ble_drv_deinit()
{
    zj_ble_adv_stop();
    zj_ble_scan_stop();
    g_ble_init_flag = BLE_STATE_DISABLE;
}

static void zj_ble_task_entry(void *params)
{
    char *name;
    uint8_t name_len;

    ln_kv_ble_app_init();

    name = zj_adapter_get_adv_name_config();
    name_len = strlen(name);
    //name max by sizeof(g_dev_name)
    if(name_len > 22)
        name_len = 22;
    ln_kv_ble_name_store(name, name_len);

    //full stack init
    ln_ble_stack_init();

    ln_ble_app_init();

    zj_adapter_post_event(ADAPT_EVT_BLE_INIT_DONE,NULL,NULL,0);
    g_ble_init_flag = BLE_STATE_ENABLE;
    while(1)
    {
        OS_Delay(10);
    }
}

/*******************************************************/
void zj_ble_drv_init()
{
    if(BLE_STATE_UNINITIALIZED == g_ble_init_flag) {
        if(OS_OK != OS_ThreadCreate(&g_zj_ble_app_thread, "ZjBleAPP", zj_ble_task_entry, NULL, OS_PRIORITY_BELOW_NORMAL, ZJ_BLE_TASK_STACK_SIZE)) 
        {
            LN_ASSERT(1);
        }
    } else if(BLE_STATE_DISABLE == g_ble_init_flag) {
        zj_adapter_post_event(ADAPT_EVT_BLE_INIT_DONE,NULL,NULL,0);
        g_ble_init_flag = BLE_STATE_ENABLE;
    } else {
        //had inited, ignore
    }
}


