#include "qcloud_iot_export.h"
#include "qcloud_iot_import.h"

#include "ble_qiot_common.h"
#include "ble_qiot_import.h"
#include "ble_qiot_log.h"

#include "mac/ble/hl/api/gattm_task.h"

#include "freertos_common.h"
#include "ble_arch/arch.h"
#include "hal/hal_flash.h"
#include "usr_app.h"

/**************************************************************************************/
/*                            LLSync LN882H                                           */
/**************************************************************************************/
enum {
    LLSYNC_DECL_CHAR_DEVICE_INFO = 0, /* write(micro_program -> ln882h)  */
    LLSYNC_CHAR_VAL_DEVICE_INFO  = 1,
    
    LLSYNC_DECL_CHAR_EVENT       = 2, /* notify(ln882h -> micro_program) */
    LLSYNC_CHAR_VAL_EVENT        = 3,
    
    LLSYNC_CLIENT_CHAR_CFG_DSCR  = 4, /* CCCD (micro_program -> ln882h)  */
};

#define DEVICE_NAME                  ("q")
#define DEVICE_NAME_LEN              (sizeof(DEVICE_NAME))
#define ADV_DATA_MAX_LENGTH          (31)

extern uint8_t  adv_actv_idx;
uint8_t  llsync_conid  = 0xFF;
uint16_t llsync_mtu    = 23; // TODO: 128

#ifdef _FIMRWARE_SDK_V_1_8_
static const ln_attm_desc_t g_gap_service_atts[] = {
    [LLSYNC_DECL_CHAR_DEVICE_INFO] = {
        .uuid = { 0x03, 0x28 },
        .perm = PERM_MASK_RD,
        .max_size = 0,
        .ext_perm = (0 <<  PERM_POS_UUID_LEN),
    },
    [LLSYNC_CHAR_VAL_DEVICE_INFO] = {
        .uuid = { IOT_BLE_UUID_DEVICE_INFO & 0xFF, (IOT_BLE_UUID_DEVICE_INFO >> 8) & 0xFF},  //0xFF, 0xE1
        .perm = PERM_MASK_WRITE_REQ,
        .max_size = BLE_QIOT_EVENT_MAX_SIZE, // TODO:???
        .ext_perm = (2 << PERM_POS_UUID_LEN),
    },

    [LLSYNC_DECL_CHAR_EVENT] = {
        .uuid = { 0x03, 0x28 },
        .perm = PERM_MASK_RD,
        .max_size = 0,
        .ext_perm =  (0 << PERM_POS_UUID_LEN),
    },
    [LLSYNC_CHAR_VAL_EVENT] = {
        .uuid = {IOT_BLE_UUID_EVENT & 0xFF, (IOT_BLE_UUID_EVENT >> 8) & 0xFF},   // 0xFFE3
        .perm = PERM_MASK_NTF,
        .max_size = BLE_QIOT_EVENT_MAX_SIZE,
        .ext_perm =  (2 << PERM_POS_UUID_LEN),
    },

    [LLSYNC_CLIENT_CHAR_CFG_DSCR] = {
        .uuid = { 0x02, 0x29 },
        .perm = PERM_MASK_WRITE_REQ|PERM_MASK_RD,
        .max_size = 0,
        .ext_perm = 0,
    },
};

ln_trans_svr_desc_t g_att_desc = {
    0
};
#else
static struct gattm_att_desc g_gap_service_atts[] = {
    [LLSYNC_DECL_CHAR_DEVICE_INFO] = {
        .uuid = { 0x03, 0x28 },
        .perm = PERM_MASK_RD,
        .max_len = 0,
        .ext_perm = (0 <<  PERM_POS_UUID_LEN),
    },
    [LLSYNC_CHAR_VAL_DEVICE_INFO] = {
        .uuid = { IOT_BLE_UUID_DEVICE_INFO & 0xFF, (IOT_BLE_UUID_DEVICE_INFO >> 8) & 0xFF},  //0xFF, 0xE1
        .perm = PERM_MASK_WRITE_REQ,
        .max_len = BLE_QIOT_EVENT_MAX_SIZE, // TODO:???
        .ext_perm = (2 << PERM_POS_UUID_LEN),
    },

    [LLSYNC_DECL_CHAR_EVENT] = {
        .uuid = { 0x03, 0x28 },
        .perm = PERM_MASK_RD,
        .max_len = 0,
        .ext_perm =  (0 << PERM_POS_UUID_LEN),
    },
    [LLSYNC_CHAR_VAL_EVENT] = {
        .uuid = {IOT_BLE_UUID_EVENT & 0xFF, (IOT_BLE_UUID_EVENT >> 8) & 0xFF},   // 0xFFE3
        .perm = PERM_MASK_NTF,
        .max_len = BLE_QIOT_EVENT_MAX_SIZE,
        .ext_perm =  (2 << PERM_POS_UUID_LEN),
    },

    [LLSYNC_CLIENT_CHAR_CFG_DSCR] = {
        .uuid = { 0x02, 0x29 },
        .perm = PERM_MASK_WRITE_REQ|PERM_MASK_RD,
        .max_len = 0,
        .ext_perm = 0,
    },
};
#endif


static uint8_t ch_write_handle   = 0;
static uint8_t ch_notify_handle  = 0;
static uint8_t ch_cccd_handle    = 0;
static uint8_t ch_cccd_enable    = 0;

void _char_svc_handle_set(uint8_t start_hdl)
{
    LOG(LOG_LVL_INFO, "svc start hdl = %d\r\n", start_hdl);
    ch_write_handle  = start_hdl + LLSYNC_CHAR_VAL_DEVICE_INFO + 1;
    ch_notify_handle = start_hdl + LLSYNC_CHAR_VAL_EVENT + 1;
    ch_cccd_handle   = start_hdl + LLSYNC_CLIENT_CHAR_CFG_DSCR + 1;
}

uint8_t _is_char_cccd_handle(uint8_t hdl)
{
    return (hdl == ch_cccd_handle);
}
uint8_t _is_char_write_handle(uint8_t hdl)
{
    return (hdl == ch_write_handle);
}

uint8_t _char_write_handle_get(void)
{
    return ch_write_handle;
}
uint8_t _char_notify_handle_get(void)
{
    return ch_notify_handle;
}
uint8_t _cccd_handle_get(void)
{
    return ch_cccd_handle;
}

void _char_cccd_enable_set(uint8_t en)
{
    ch_cccd_enable = en;
}

static void _app_create_adv_activity(void)
{
    adv_param_t *adv_param = &le_adv_mgr_info_get()->adv_param;
    
    adv_param->adv_type = GAPM_ADV_TYPE_LEGACY;
    adv_param->adv_prop = GAPM_ADV_PROP_UNDIR_CONN_MASK;
    ln_ble_adv_actv_creat(adv_param);
}

static void _app_set_adv_data(adv_info_s *adv)
{
    //adv data: adv length--adv type--adv string ASCII
    uint8_t raw_adv_data[ADV_DATA_MAX_LENGTH] = {
            /* flags */
            // 0x02, 0x01, 0x06,
            /* service UUID */
            0x03, 0x03, 0xF0, 0xFF
    };

    uint8_t temp_data_buf[ADV_DATA_MAX_LENGTH] = {0};
    uint8_t temp_data_len = 0;
    uint8_t index = 0;

    /* 1. temp data */
    memcpy(temp_data_buf, &adv->manufacturer_info.company_identifier, sizeof(uint16_t));
    temp_data_len = sizeof(uint16_t);
    memcpy(temp_data_buf + temp_data_len, adv->manufacturer_info.adv_data, adv->manufacturer_info.adv_data_len);
    temp_data_len += adv->manufacturer_info.adv_data_len;

    /* 2. raw data */
    index                 = 4;
    raw_adv_data[index++] = temp_data_len + 1;
    raw_adv_data[index++] = 0xFF;
    memcpy(raw_adv_data + index, temp_data_buf, temp_data_len);
    index += temp_data_len;

    raw_adv_data[index++] = strlen(DEVICE_NAME) + 1;
    raw_adv_data[index++] = 0x09;
    memcpy(raw_adv_data + index, DEVICE_NAME, strlen(DEVICE_NAME));
    index += strlen(DEVICE_NAME);

    ble_qiot_log_hex(BLE_QIOT_LOG_LEVEL_INFO, "BLE_ADV raw data", (const char *)raw_adv_data, index);

    ln_adv_data_t adv_data_param;
    adv_data_param.data = raw_adv_data;
    adv_data_param.length = index;
    ln_ble_adv_data_set(&adv_data_param);
}

static void _app_start_adv(void)
{
    ln_ble_adv_start();
}


/**************************************************************************************/
/*                            LLSync (ble_qiot_import.h)                              */
/**************************************************************************************/
int ble_get_mac(char *mac)
{
    ln_ble_mac_get((uint8_t *)mac);
    return 0;
}

void ble_services_add(const qiot_service_init_s *p_service)
{
#ifdef _FIMRWARE_SDK_V_1_8_
    g_att_desc.att_desc = &g_gap_service_atts;
    g_att_desc.start_handle = 0xFFFF;
    g_att_desc.att_count = sizeof(g_gap_service_atts)/sizeof(g_gap_service_atts[0]);
    g_att_desc.svr_uuid_len = 16;
    
    // service UUID (128bit) 0xFFF0
    memcpy(g_att_desc.svr_uuid , p_service->service_uuid128, 16);
    g_att_desc.svr_uuid[12] = p_service->service_uuid16 & 0xFF;
    g_att_desc.svr_uuid[13] = (p_service->service_uuid16 >> 8) & 0xFF;
    
    //characteristic (write) -- UUID 0xFFE1
    memcpy(g_att_desc.att_desc[LLSYNC_CHAR_VAL_DEVICE_INFO].uuid, p_service->service_uuid128, 16);
    g_att_desc.att_desc[LLSYNC_CHAR_VAL_DEVICE_INFO].uuid[12] = p_service->device_info.uuid16 & 0xFF;
    g_att_desc.att_desc[LLSYNC_CHAR_VAL_DEVICE_INFO].uuid[13] = (p_service->device_info.uuid16 >> 8) & 0xFF;

    //characteristic (notify) -- UUID 0xFFE3
    memcpy(g_att_desc.att_desc[LLSYNC_CHAR_VAL_EVENT].uuid, p_service->service_uuid128, 16);
    g_att_desc.att_desc[LLSYNC_CHAR_VAL_EVENT].uuid[12] = p_service->event.uuid16 & 0xFF;
    g_att_desc.att_desc[LLSYNC_CHAR_VAL_EVENT].uuid[13] = (p_service->event.uuid16 >> 8) & 0xFF;
    if(BLE_ERR_NONE != ln_ble_trans_svr_add(&g_att_desc))
    {
        LOG(LOG_LVL_ERROR, "ble add svr fail!!\r\n");
    }
    //set start handle
    _char_svc_handle_set(g_att_desc.start_handle);
#else
    int nb_att = sizeof(g_gap_service_atts)/sizeof(g_gap_service_atts[0]);
    struct ln_gattm_add_svc_req p_svc_desc;
    p_svc_desc.svc_desc.start_hdl = 0;
    p_svc_desc.svc_desc.perm = (2 << 5);

    // service UUID (128bit) 0xFFF0
    memcpy(p_svc_desc.svc_desc.uuid, p_service->service_uuid128, 16);
    p_svc_desc.svc_desc.uuid[12] = p_service->service_uuid16 & 0xFF;
    p_svc_desc.svc_desc.uuid[13] = (p_service->service_uuid16 >> 8) & 0xFF;

    // characteristic (write) -- UUID 0xFFE1
    memcpy(g_gap_service_atts[LLSYNC_CHAR_VAL_DEVICE_INFO].uuid, p_service->service_uuid128, 16);
    g_gap_service_atts[LLSYNC_CHAR_VAL_DEVICE_INFO].uuid[12] = p_service->device_info.uuid16 & 0xFF;
    g_gap_service_atts[LLSYNC_CHAR_VAL_DEVICE_INFO].uuid[13] = (p_service->device_info.uuid16 >> 8) & 0xFF;

    // characteristic (notify) -- UUID 0xFFE3
    memcpy(g_gap_service_atts[LLSYNC_CHAR_VAL_EVENT].uuid, p_service->service_uuid128, 16);
    g_gap_service_atts[LLSYNC_CHAR_VAL_EVENT].uuid[12] = p_service->event.uuid16 & 0xFF;
    g_gap_service_atts[LLSYNC_CHAR_VAL_EVENT].uuid[13] = (p_service->event.uuid16 >> 8) & 0xFF;

    p_svc_desc.svc_desc.nb_att = nb_att;
    p_svc_desc.svc_desc.atts = g_gap_service_atts;
    ln_app_gatt_add_svc(&p_svc_desc);
#endif
}

ble_qiot_ret_status_t ble_advertising_start(adv_info_s *adv)
{
    _app_create_adv_activity();
    _app_set_adv_data(adv);
    _app_start_adv();
    return BLE_QIOT_RS_OK;
}

ble_qiot_ret_status_t ble_advertising_stop(void)
{
    ln_ble_adv_stop();
    return BLE_QIOT_RS_OK;
}

uint16_t ble_get_user_data_mtu_size(e_system type)
{
    return llsync_mtu;
}

ble_qiot_ret_status_t ble_send_notify(uint8_t *buf, uint8_t len)
{
    ln_trans_svr_send_t svc_send_cmd;
    svc_send_cmd.conn_idx = 0;
    svc_send_cmd.data = buf;
    svc_send_cmd.len = len;
    svc_send_cmd.hdl = _char_notify_handle_get();
    ln_ble_trans_svr_ntf(&svc_send_cmd);
    return BLE_QIOT_RS_OK;
}

#if (BLE_QIOT_BUTTON_BROADCAST || BLE_QIOT_SUPPORT_OTA)
  ble_timer_t ble_timer_create(uint8_t type, ble_timer_cb timeout_handle) {
      return (ble_timer_t)NULL;
  }
  
  ble_qiot_ret_status_t ble_timer_start(ble_timer_t timer_id, uint32_t period) {
      return BLE_QIOT_RS_OK;
  }
  
  ble_qiot_ret_status_t ble_timer_stop(ble_timer_t timer_id) {
      return BLE_QIOT_RS_OK;
  }
  
  ble_qiot_ret_status_t ble_timer_delete(ble_timer_t timer_id) {
      return BLE_QIOT_RS_OK;
  }
#endif /* (BLE_QIOT_BUTTON_BROADCAST || BLE_QIOT_SUPPORT_OTA) */

#ifdef BLE_QIOT_LLSYNC_STANDARD
  DeviceInfo sg_temp_devinfo = {0};

  int ble_get_product_id(char *product_id)
  {
      int ret = 0;
      memset(&sg_temp_devinfo, 0, sizeof(DeviceInfo));
      ret = HAL_GetDevInfo(&sg_temp_devinfo);
      if (ret != QCLOUD_RET_SUCCESS) {
          return -1;
      }

      memcpy(product_id, sg_temp_devinfo.product_id, BLE_QIOT_PRODUCT_ID_LEN);
      return 0;
  }

  int ble_get_device_name(char *device_name)
  {
      int ret = 0;
      memset(&sg_temp_devinfo, 0, sizeof(DeviceInfo));  
      ret = HAL_GetDevInfo(&sg_temp_devinfo);
      if (ret != QCLOUD_RET_SUCCESS) {
          return -1;
      }
  
      memcpy(device_name, sg_temp_devinfo.device_name, strlen(sg_temp_devinfo.device_name));
      return strlen(sg_temp_devinfo.device_name);
  }

  int ble_get_psk(char *psk)
  {
      int ret = 0;
      memset(&sg_temp_devinfo, 0, sizeof(DeviceInfo));  
      ret = HAL_GetDevInfo(&sg_temp_devinfo);
      strcpy(psk, sg_temp_devinfo.product_secret);
      return 0;  
  }

  #if BLE_QIOT_DYNREG_ENABLE

    int ble_set_psk(const char *psk, uint8_t len)
    {
        return 0;
    }

    int ble_get_product_key(char *product_secret)
    {
        return 0;
    }
  #endif /* BLE_QIOT_DYNREG_ENABLE */
    
  int ble_write_flash(uint32_t flash_addr, const char *write_buf, uint16_t write_len)
  {
      hal_flash_program(flash_addr, write_len, (uint8_t *)write_buf);
      return write_len;
  }

  int ble_read_flash(uint32_t flash_addr, char *read_buf, uint16_t read_len)
  {
      hal_flash_read_by_cache(flash_addr, read_len, (uint8_t *)read_buf);
      return read_len;
  }

  #if BLE_QIOT_SECURE_BIND

    void ble_secure_bind_user_cb(void)
    {
        return 0;
    }

    void ble_secure_bind_user_notify(uint8_t result)
    {
        return 0;
    }
  #endif /* BLE_QIOT_SECURE_BIND */

  #if BLE_QIOT_SUPPORT_OTA

    uint8_t ble_ota_is_enable(const char *version)
    {
        return 0;
    }

    uint32_t ble_ota_get_download_addr(void)
    {
        return 0;
    }

    int ble_ota_write_flash(uint32_t flash_addr, const char *write_buf, uint16_t write_len)
    {
        return 0;
    }
  #endif /* BLE_QIOT_SUPPORT_OTA */

  void ble_qiot_dev_start(void)
  {
//      extern void ble_app_init(void);
//      ble_app_init();
  }
#endif  /* BLE_QIOT_LLSYNC_STANDARD */


#if BLE_QIOT_LLSYNC_CONFIG_NET
  #include "qcloud_wifi_config.h"
  
  extern int   port_wifi_set_connected_param(char *ssid, uint8_t ssid_len, char *pwd, uint8_t pwd_len);
  extern char *port_wifi_get_connected_ap_ssid(void);
  extern char *port_wifi_get_connected_ap_pwd(void);

  ble_qiot_ret_status_t ble_combo_wifi_mode_set(BLE_WIFI_MODE mode)
  {
      //TODO:
      //1. set wifi mode
      
      //2. report to MicroProgrom
      ble_event_report_wifi_mode(0);
      return BLE_QIOT_RS_OK;
  }
  
  ble_qiot_ret_status_t ble_combo_wifi_info_set(const char *ssid, uint8_t ssid_len, const char *passwd, uint8_t passwd_len)
  {
      //TODO:
      //1. set wifi config info
      port_wifi_set_connected_param((char *)ssid, ssid_len, (char *)passwd, passwd_len);
      
      //2. report to MicroProgrom
      ble_event_report_wifi_info(0);
      
      //3. BLE WIFI_CONFIG_SUCCESS
      set_bt_combo_config_state(WIFI_CONFIG_SUCCESS);
      return BLE_QIOT_RS_OK;
  }
  
  ble_qiot_ret_status_t ble_combo_wifi_connect(void)
  {
      const char * ssid = port_wifi_get_connected_ap_ssid();
      const char * psw  = port_wifi_get_connected_ap_pwd();
      HAL_Wifi_StaConnect(ssid, psw, 0);
      return BLE_QIOT_RS_OK;
  }
  
  ble_qiot_ret_status_t ble_combo_wifi_token_set(const char *token, uint16_t token_len)
  {
      qiot_device_bind_set_token(token);
      return BLE_QIOT_RS_OK;
  }

  ble_qiot_ret_status_t ble_combo_wifi_log_get(void)
  {
      app_send_ble_dev_log();
      app_send_ble_error_log();
      return BLE_QIOT_RS_OK;
  }
#endif  /* BLE_QIOT_LLSYNC_CONFIG_NET */

