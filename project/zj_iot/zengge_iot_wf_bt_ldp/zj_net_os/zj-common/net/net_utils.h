
#ifndef NET_UTILS__H
#define NET_UTILS__H

#define NET_TYPE_DEFAULT           0         /*出厂默认网络:WIFI AP + BLE(if ble is supported)*/
#define NET_TYPE_NET_CONNECT       1         /*触发配网:WIFI STA + BLE(if ble is supported)*/
#define NET_TYPE_ONLY_BLE          2         /*only BLE(if ble is supported)*/
#define NET_TYPE_ONLY_WIFI         3         /*only WIFI*/
#define NET_TYPE_ONLY_SCAN         4
#define NET_TYPE_ONLY_WIFI_NOBTRMT 5
#define NET_TYPE_OFF               6
#define NET_TYPE_ENABLE_RECFG      7
#define NET_TYPE_MAX               8

typedef void (*net_cfg_cb_t)(uint8_t type);

#ifdef APP_CONFIG_DYNAMIC_SYNC_ADV
typedef void (*dynamic_sync_adv_cb_t)(uint8_t* group_id,uint8_t mode_type,uint8_t mode);
void register_dynamic_sync_cmd_callback(dynamic_sync_adv_cb_t cb);
void dynamic_sync_payload_update(uint8_t *group_id,uint8_t mode_type,uint8_t mode);
#endif

  /**
 * @brief 网络类型配置
 * @note 
 * @param[in] type : 网络类型
 * - NET_TYPE_ONLY_BLE:
 * - NET_TYPE_ONLY_WIFI:
 * @param[out] none
 *
 * @return none
 */
void net_type_init();
void net_type_set_config(uint8_t type);
uint8_t net_type_get_config();
void register_net_type_cfg_event(net_cfg_cb_t cb);
void connect_to_routing_Indicator();

#endif