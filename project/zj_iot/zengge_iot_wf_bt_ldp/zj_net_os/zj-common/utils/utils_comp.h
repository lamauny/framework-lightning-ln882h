
#ifndef UTILS_COMP_H_ 
#define UTILS_COMP_H_

void zj_once_tcp_connected_set();
void zj_once_cfg_net_set();
/**
 * @brief 查询是否曾经配置过网络
 * @note  
 * @param[in] none
 * @return 1：是  0：否
 */
bool is_once_cfg_net();
bool is_once_tcp_connected();

#endif
