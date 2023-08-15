#ifndef __ATCMD_H__
#define __ATCMD_H__



#ifdef __cplusplus
extern "C" {
#endif

typedef void (*udp_data_cb_t)(char *data,int len);
/**
 * @brief 注册udp数据回调
 * @note 
 * @param[in] cb
 * @param[out] none
 *
 * @return none
 */
void udp_data_regist_cb(udp_data_cb_t cb);

void AT_CMD_parser(char *pdata,int data_len);
void connect_to_routing_Indicator();
void AT_set_iic_param_addr(uint32_t addr);

#ifdef __cplusplus
}
#endif


#endif /*#ifndef __TCP_PERF_H__*/

