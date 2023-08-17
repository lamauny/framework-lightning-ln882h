
#ifndef NET_UTILS__H
#define NET_UTILS__H

#define NET_TYPE_FACTORY_WIFI_BLE_RMT           0         /*出厂默认网络:WIFI AP & BLE & RMT*/
#define NET_TYPE_STA_BLE_RMT                    1         /*已配网:WIFI STA & BLE & RMT*/
#define NET_TYPE_BLE_RMT                        2         /*已配网:BLE & RMT*/
#define NET_TYPE_WIFI_RMT                       3         /*已配网:WIFI (AP | STA) & RMT*/
#define NET_TYPE_RMT                            4         /* only RMT */
#define NET_TYPE_FACTORY_WIFI                   5         /* 出厂默认网络:WIFI AP */
#define NET_TYPE_OFF                            6         /* 关闭所有的:WIFI & BLE & RMT */
#define NET_TYPE_ENABLE_RECFG                   7         /*BLE 允许重新配网*/
#define NET_TYPE_MAX                            8

#ifndef CONFIG_NET_TYPE_DEFAULD
#define CONFIG_NET_TYPE_DEFAULD    NET_TYPE_FACTORY_WIFI_BLE_RMT
#endif

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
 * - NET_TYPE_BLE_RMT:
 * - NET_TYPE_WIFI_RMT:
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