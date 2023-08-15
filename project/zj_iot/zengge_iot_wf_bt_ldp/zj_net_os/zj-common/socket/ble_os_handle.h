
#ifndef _BLUFI_HANDLE_H_
#define _BLUFI_HANDLE_H_

#ifdef __cplusplus
extern "C" {
#endif
//https://docs.qq.com/sheet/DREdlT0xiRHlpQWFR?tab=35zi5z
typedef enum {
    BLUFI_CMDID_NULL = 0x00,
    BLUFI_CMDID_WIFI_SCAN,            /* 请求查询路由器列表 */
    BLUFI_CMDID_DISCONNECT_WIFI,      /* 请求断开wifi连接 */
    BLUFI_CMDID_WIFI_STATE_GET,       /* 请求wifi连接状态 */
    BLUFI_CMDID_ROUTER_SET,           /* 请求配置路由器 */
    BLUFI_CMDID_CLOUD_SET,            /* 请求配置云端 */
    BLUFI_CMDID_LOG_QUERY,            /* 请求查询运行log */
    BLUFI_CMDID_NET_QUERY,            /* 请求网络状态 */
    BLUFI_CMDID_FACTORY_SETTING,      /* 请求恢复出厂设置 */
    BLUFI_CMDID_NETTYPE_SETTING,      /* 请求配置网络类型*/
    BLUFI_CMDID_CONTROL_WITH_RES,     /* 十六进制控制协议，带返回 */
    BLUFI_CMDID_CONTROL_NO_RES,       /* 十六进制控制协议，不带返回*/
    BLUFI_CMDID_AUTO_REPORT,          /* 设备自动上报设备状态 */  
    BLUFI_CMDID_NET_DIAGNOSIS   = 0x10,   /* 开始网络诊断 */
    BLUFI_CMDID_QUERY_DIAGNOSIS = 0x11,   /* 查询网络诊断状态 */
    BLUFI_CMDID_NET_REPAIR      = 0x12,   /*  网络修复 */
    BLUFI_CMDID_JSON_RECV,
    BLUFI_CMDID_JSON_SEND      
} cmd_id_t;

typedef void (*ble_cfg_net_data_t)(char *data,int len);
 /**
 * @brief 注册蓝牙配置网络数据回调
 * @note 
 * @param[in] cb
 * @param[out] none
 *
 * @return none
 */
void ble_cfg_net_inf_regist_cb(ble_cfg_net_data_t cb);

#define WLAN_SCAN_BUF_SIZE   2048

void zj_ble_pro_init();

 /**
 * @brief wifi连接状态上报
 * @note 
 * @param[in] event_id : cmd id
 * @param[out] none
 *
 * @return none
 */
void blufi_wlan_status_report(uint8_t cmd_id,uint8_t status);
 /**
 * @brief 控制协议返回
 * @note 
 * @param[in] hex : 十六进制返回数据
 * @param[in] len : 
 * @param[in] event_id : cmd id
 * @param[out] none
 *
 * @return none
 */
int zj_ble_payload_send(uint8_t *hex,int len,uint8_t cmd_id);
 /**
 * @brief 控制协议返回
 * @note 
 * @param[in] data : json 
 * @param[in] len : 
 * @param[out] none
 *
 * @return none
 */
int zj_ble_json_send(char *data,int len);
 /**
 * @brief 查询是否配网中
 * @note 
 * @param[in] none
 *
 * @return 1 : 配网中 else 没有
 */
uint8_t blufi_get_configNet_flag();
void blufi_clear_configNet_flag();
void network_info_query(uint8_t option);

#ifdef __cplusplus
}
#endif

#endif /* _COMMAND_H_ */

