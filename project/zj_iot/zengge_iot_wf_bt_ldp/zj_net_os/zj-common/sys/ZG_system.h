#ifndef _ZG_SYS_H_
#define _ZG_SYS_H_

/*
 task priority
 tcp sever 6 
 udp 5
 tcp client 7
 store  2
 system 1

*/

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "ZG_sys_config.h"

#include "WIFI_type.h"
#include "zj_adapt_config.h"
#include "zj_adapt_api.h"

#include "WIFI_config.h"
#include "ble_transfer.h"
#include "ble_os_handle.h"
#include "net_utils.h"

#include "report_type.h"
#include "json_char_convert.h"
#include "ZG_utils.h"
#include "color.h"
#include "timeStamp.h"
#include "utils_linked_list.h"
#include "zJSON.h"
#include "endian.h"

#include "udp_perf.h"
#include "tcp_perf.h"
#include "tcp_client.h"

#include "proto_data_event.h"
#include "proto_parser_pack.h"

#include "debug_info.h"

#include "encryption.h"
#include "os_key.h"

#include "AT_CMD.h"
#include "debug_info.h"

#include "bt_remote.h"
#include "pub_btr.h"
#include "btr.h"
#include "utils_zlog.h"
#include "cmd_cache.h"
#include "pwr_on_setting.h"
#include "new_clock.h"
#include "net_diagnosis.h"
#include "bridge_ble_central_evt.h"
#include "utils_tcplist.h"

#include "fota.h"
#include "tlk_chip_ota.h"
#include "zjos_ota.h"
#include "uart_ota.h"

#include "rmt_scene_list.h"
#include "rmt_scene.h"
#include "utils_comp.h"
#include "product_test.h"
#include "default_cfg_evt.h"

#include "zj_ping.h"

#include "api_compatibility.h"

#include "os_version.h"


#ifdef CONFIG_SUPPORT_DYNAMIC_SYNC
#include "sync_hbm.h"
#include "sync_mdls.h"
#endif


#if USE_ZG_KV_MODULE_OPTION
   #include "zj-common/store_kv/include/store_kv_api.h"
#endif

#ifndef IS_ESP_PLATFORM
typedef int32_t esp_err_t;

/* Definitions for error constants. */
#define ESP_OK          0       /*!< esp_err_t value indicating success (no error) */
#define ESP_FAIL        -1      /*!< Generic esp_err_t code indicating failure */

#define BIT0    0x00000001
#endif


void zj_set_app_icon_id(uint8_t id);
uint8_t zj_get_app_icon_id();

void zj_storage_module_start();
void zj_monitor_start(uint32_t internal_time_sec);
 /**
 * @brief Configure device mid
 * @author hings
 * @note  
 * @param[in] version : 
 * @return 0:success  -1:fail
 */
esp_err_t zj_creat_device_mid(uint8_t version);
 /**
 * @brief Configure device code and control protocol version
 * @author hings
 * @note  
 * @param[in] dev_code : evice code
 * @param[in] pro_ver : protocol version
 * @param[in] chip_type : ble chip_type
 * @return 0:success  -1:fail
 */
esp_err_t zj_device_info_conf(uint16_t dev_code,uint8_t pro_ver,uint8_t chip_type);
 /**
 * @brief Configure firmware version information
 * @author hings
 * @note   such as 33_01_20190220_ZG-LX-IR    ZG as customer_name  LX as chip_manufacturer  IR as ver_type
 * @param[in] ver : Version number
 * @param[in] time : 20190220
 * @param[in] customer_name : Customer name,such as "ZG"
 * @param[in] chip_manufacturer : chip manufacturer name,such as "LX"
 * @param[in] ver_type : Firmware type, such as "IR" or "RF"
 * @return 0:success  -1:fail
 * Example usage:
   <pre>
    zj_device_version_conf(1,"20200406,"ZG","ZW","IR");
    For example, the device code is 0x33,then a build number will be generated inside the function:
    33_01_20200406_ZG-ZW-IR
   </pre>
 */
esp_err_t zj_device_version_conf(int ver,char *time,char *customer_name,char *chip_manufacturer,char *ver_type);
esp_err_t zj_bridge_version_conf(char *ver_string,char *time,char *customer_name,char *chip_manufacturer,char *ver_type);

void zj_device_mcu_ver_conf(char * ver);

char *zj_get_fw_version();
uint16_t zj_get_fw_version_hex();
char *zj_get_device_mid();
uint8_t zj_get_device_mid_version();
char *zj_get_mcu_version();
uint16_t zj_get_device_code();
uint8_t zj_get_chip_type();
uint8_t zj_get_device_protocol_version();

typedef enum {
    FACTORY_SETTING_EVENT = 0,   //恢复出厂设置事件
    CONNECTED_ROUTER_EVENT,      //连接路由器成功事件
    DISCONNECTED_ROUTER_EVENT,   //断开路由器事件
    RECONNECTED_ROUTER_EVENT,    //重连路由器事件
    CONNECTING_ROUTER_EVENT , 
    OTA_START_EVENT,
    OTA_PROCESS_EVENT,           
    OTA_SUCCESS_EVENT,
    OTA_FAIL_EVENT,
    DISTRIBUTION_NETWORK_INDICATION_EVENT,//APP配置网络后连接路由器成功事件
    CLOUD_CONNECTED_EVENT,
    DISTRIBUTION_NETWORK_EVENT,         //APP配置网络事件
    BLE_CONNECTED_EVENT,
    BLE_DISCONNECTED_EVENT,
    DEV_TRIGGER_TEST_EVENT,
    DEV_TRIGGER_AGING_EVENT,
    NO_ENC_KEY_FOUND_EVENT,
    RTC_PROOFREADING_EVENT,
    ENC_KEY_CHANGE_EVENT
} ZG_system_event_t;

/**
  * @brief  Application specified event callback function
  *
  * @param  void *ctx : reserved for user
  * @param  system_event_t *event : event type defined in this file
  *
  * @return ESP_OK : succeed
  * @return others : fail
  */
typedef esp_err_t (*ZG_system_event_cb_t)(void *ctx, ZG_system_event_t *event);

/**
  * @brief  Initialize event loop
  *         Create the event handler and task
  *
  * @param  system_event_cb_t cb : application specified event callback, it can be modified by call esp_event_set_cb
  * @param  void *ctx : reserved for user
  *
  * @return ESP_OK : succeed
  * @return others : fail
  */
esp_err_t register_zjos_event(ZG_system_event_cb_t cb, void *ctx);
esp_err_t zjos_event_post(ZG_system_event_t event);


#ifdef __cplusplus
}
#endif



#endif 