#ifndef PUBLIC_BTR_H
#define PUBLIC_BTR_H

#include "bt_remote.h"


void pub_btr_parser(uint8_t *data);
/**
 * @brief 注册公版遥控回调事件
 * @note 
 * @param[in]  pub_cb 回调事件
 *
 * @return none
 */
void pub_btr_regist_data_event(btr_event_cb_t cb);
/**
 * @brief 注册公版遥控控透传数据回调事件
 * @note 
 * @param[in]  pub_cb 回调事件
 *
 * @return none
 */
int pub_btr_regist_tt_data_event(btr_event_cb_t cb);
  /**
 * @brief 获取公版遥控绑定的数量
 * @note 
 * @param[in] none
 * @param[out] none
 *
 * @return 绑定的数量
 */
uint8_t pub_btr_get_bind_count();
  /**
 * @brief 删除公版遥控所有已经绑定的遥控
 * @note 
 * @param[in] none
 * @param[out] none
 *
 * @return none
 */
void pub_btr_clear_all_bind();
  /**
 * @brief 设置公版遥控允许接收的设备码
 * @note 
 * @param[in] recv_type_choost_t  type_list type_num
 * @param[out] none
 *
 * @return none
 */
void pub_btr_set_recv_type(recv_type_choost_t choose_type,uint8_t type_list[10],uint8_t type_num);;

#endif