#ifndef __PROTO_DATA_TYPE_H__
#define __PROTO_DATA_TYPE_H__


#ifdef __cplusplus
extern "C" {
#endif

#define COMMAND_HEAD_LENGTH              10

/* 统一命令2.0协议头 : https://docs.qq.com/sheet/DRER0bFZOcFdJQUta?tab=BB08J2 */
#define COMMAND_V2_HEAD_CONTROL          0xE0    /* 统一命令2.0:统一控制命令头 */
#define COMMAND_V2_HEAD_CONTROL1         0xE1    /* 统一命令2.0:幻彩命令头 */
#define COMMAND_V2_HEAD_QUERY            0xEA    /* 统一命令2.0:设备信息查询命令头 */
#define NEW_CMD_UNIFIED_PWR_CFG_SET      0x0B    /* E0 0B:通电反应设置 */
#define NEW_CMD_UNIFIED_PWR_CFG_QUERY    0x0C    /* E0 0C:通电反应查询 */
#define NEW_CMD_UNIFIED_CLOCLK_SET       0x05    /* E0 05:（闹钟设置) */
#define NEW_CMD_UNIFIED_CLOCK_QUERY      0x06    /* E0 06（闹钟查询) */
#define NEW_CMD_UNIFIED_CLOUD_SET        0x17    /* E0 17:切换服务器域名 */
#define NEW_CMD_UNIFIED_CLOUD_QUERY      0x18    /* E0 18:查询服务器域名 */
#define NEW_CMD_UNIFIED_MUTI_MERGE_CMD   0x19    /* E0 19:多命令合并发送 */
#define NEW_CMD_UNIFIED_DBG_REPORT_NOW_CMD   0x1B    /* E0 1B: */
/*通用控制命令协议头: https://docs.qq.com/sheet/DTVZkSWlnaVptcHlU?tab=9jts84 */
#define GCCMD_TIME_SET         0x10 /* 时间设置命令头 */
#define GCCMD_TIME_QUERY       0x11 /* 时间查询命令头 */

#ifndef CONFIG_PDC_QUEUE_DEPTH
#define CONFIG_PDC_QUEUE_DEPTH   10
#endif

typedef enum {

    TCP_LOCAL_DATA_EVENT = 0, //local tcp data
    CLOUD_DATA_EVENT,   //cloud data
    BT_DATA_EVENT_RES,  //ble data with returen
    BT_DATA_EVENT_NORES, //ble data no return
    OTHER_DATA_EVENT,
    BT_DATA_EVENT_JSON,
    UDP_DATA_EVENT,
    DBG_DATA_REP_EVENT,
    BT_DATA_SRC_DATA_EVENT,
    BRMT_DATA_EVENT
} data_event_id_t;

typedef enum {

    REPORT_BLE,                       //Report on ble channel 
    REPORT_TCP_CLOUD,                 //Report on tcp client cloud channel : data type is hex
    REPORT_TCP_LOCAL,                 //Report on tcp local  channel  : data type is hex
    REPORT_TCP_CLOUD_JSON, 
    #ifdef APPCONFIG_SUPPORT_HTTP_DL_CMD
    REPORT_HTTP_DL_RESULT,
    REPORT_HTTP_DL_PROGRESS
    #endif
} report_dst_t;

typedef struct 
{
  data_event_id_t event_id;
  bool support_transparent_head;
  uint8_t sn;
  uint8_t version;
  uint8_t  *data;
  uint16_t data_len;
  uint8_t data_format;
  #ifdef APPCONFIG_SUPPORT_HTTP_DL_CMD
  bool is_http_download_cmd;
  #endif
}pro_data_event_t;

typedef struct 
{
	uint8_t head[10];        //Header to parse
	uint16_t body_len[10];    //Length to parse
	uint8_t number;          //Number to parse  <= 10
}pro_param_t;

typedef struct{

  data_event_id_t event_id;
  uint16_t data_len;
  uint8_t  *data;
}data_buf_t;

int proto_data_post(data_buf_t *pbuf);

int proto_wait_dbg_repo_response(uint32_t time);

typedef int (*ZG_data_event_cb_t)(pro_data_event_t *event);

 /**
 * @brief 注册交互数据回调事件
 * @note 
 * @param[in] cb : 回调事件
 * @param[out] none
 *
 * @return 0 - success else fail
 */
int register_app_data_event(ZG_data_event_cb_t cb);
 /**
 * @brief 推送正文交互数据事件,会进入交互数据回调事件
 * @note 
 * @param[in] data : data
 * @param[in] len : data length
 * @param[in] id : data id
 * @param[out] none
 *
 * @return 0 - success else fail
 */
void ZG_post_app_data_event(uint8_t *data,int len,data_event_id_t id);
/**
 * @brief 交互数据返回
 * @note 
 * @param[in] channel : 返回通道
 * @param[in] data : 数据
 * @param[in] len : 
 * @param[out] none
 *
 * @return none
 */
void ZG_return_data(pro_data_event_t *pro_data);
/**
 * @brief 状态报告发送
 * @note 
 * @param[in] dst : 报告通道
 * @param[in] data : 数据
 * @param[in] pro:报头协议版本
 * @param[out] none
 *
 * @return 0 - success else fail
 */
void ZG_report_data(report_dst_t dst,uint8_t *data,int len,uint8_t pro);
/**
 * @brief 设置数据拼包处理器参数
 * @author hings
 * @param[in]  overlap_param_t *param ，
 * @return none
 */
int pkg_pack_set_config(pro_param_t *param);

/**
 * @brief 设置叠包解析处理器参数
 * @author hings
 * @param[in]  overlap_param_t *param ，
 * @return none
 */
void overlap_parser_set_config(pro_param_t *param);
 /**
 * @brief 本地数据解析处理
 * @note 
 * @param[in] data : data
 * @param[in] len : data length
 * @param[in] id : data id
 * @param[out] none
 *
 * @return PROTOCOL_TRANSPARENT or PROTOCOL_NOMAL
 */
int local_protocol_data_parser(uint8_t *data,int len,data_event_id_t id);
/**
 * @brief 云端数据解析处理
 * @note 
 * @param[in] data : data
 * @param[in] len : data length
 * @param[in] id : data id
 * @param[out] none
 *
 * @return cloud_data_type_t : 云端数据类型
 */
int cloud_protocol_data_parser(uint8_t *data,int len);
int udp_protocol_data_parser(uint8_t *data,int len);

#ifdef __cplusplus
}
#endif



#endif 