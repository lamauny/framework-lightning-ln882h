#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__



#ifdef __cplusplus
extern "C" {
#endif

#define PARSER_SUCCESS         0
#define PARSER_HEAD_FAIL     -1
#define PARSER_HEAD_CRC_FAIL -2
#define PARSER_DATA_FAIL      -3
#define PARSER_DATA_CRC_FAIL  -4

#define BYTE_FORMAT    1
#define STRING_FORMAT  2
#define JSON_FORMAT    3

#define PACK_CONNECT_REQUESE  0x00
#define PACK_PING_CHECK       0x01
#define PACK_DEICE_REPORT     0x02
#define PACK_DBG_REPORT       0x03
#define PACK_LED_PROTOCOL     0xc3
#define PACK_FOTA_RESPONSE    0xd0
#define PACK_FOTA_SUCCESS     0xd1

#define CLOUD_QUERY  0xf0
/**
* @brief 数据校验
* @author hings
* @note 
* @param[in]  pbuff  : 输入数据，
* @param[in] length  : length of pbuff
* @return 0 : success ,else fail
*/
int  data_crc_check(uint8_t  *pbuff,uint16_t length);
/**
* @brief 求数据校验和
* @author hings
* @note 
* @param[in]  pbuff  : 输入数据，
* @param[in] length  : length of pbuff
* @return checkSum   : 校验和
*/
uint8_t data_checkSum(uint8_t * dat,unsigned char len);
/**
* @brief 将发送内容封装成与服务器交互的报文，具体参考 Wi-Fi_通信协议_2.0
* @author hings
* @note 
* @param[in]  char type :   数据类型
*             - PACK_connet_request 服务器连接请求
*             - PACK_ping_check     心跳包
*             - PACK_device_report  设备上报状态
*             - PACK_Ledprotocol    灯的控制协议
*             - PACK_FOTA_reponse   OTA进度上报
* @param[in] format : 数据格式
*             - BYTE_Format
*             - String_Format
*             - JSON_Format
* @param[in] body : 发送内容
* @param[in] output : 完整报文
* @param[in] length : body的长度
* @param[in] length : 协议版本
* @return 0 : success ,else fail
*/
int  cloud_reponse_data_pack(char type,char format, const uint8_t *body,uint8_t *output,uint16_t length,uint8_t pro);
/**
* @brief 将发送内容封装成与本地交互的报文
* @author hings
* @note 
* @param[in]  char type :   数据类型
*             - PACK_connet_request 服务器连接请求
*             - PACK_ping_check     心跳包
*             - PACK_device_report  设备上报状态
*             - PACK_Ledprotocol    灯的控制协议
*             - PACK_FOTA_reponse   OTA进度上报
* @param[in] format : 数据格式
*             - BYTE_Format
*             - String_Format
*             - JSON_Format
* @param[in] body : 发送内容
* @param[in] output : 完整报文
* @param[in] length : body的长度
* @param[in] length : 协议版本
* @return 0 : success ,else fail
*/
int  local_reponse_data_pack(uint16_t opcode,uint8_t sn, const uint8_t *body,uint8_t *output,uint16_t length,uint8_t pro);
int  http_dl_reponse_data_pack(char type,const uint8_t *body,uint8_t *output,uint16_t length);

#ifdef __cplusplus
}
#endif


#endif 
