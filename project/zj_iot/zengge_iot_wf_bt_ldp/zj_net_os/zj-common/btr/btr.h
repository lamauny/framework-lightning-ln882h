
#ifndef _BTR_H
#define _BTR_H

#include "bt_remote.h"

/**
 * @brief 遥控设置接收的遥控类型
 * @note 
 * @param[in] choose_type：接收的选择，是否接收所有的还是接收设置的
 * @param[in] type_list：接收类型列表
 * @param[in] type_num：类型数量
 * @param[out] none
 *
 * @return none
 */
void btr_set_recv_type(recv_type_choost_t choose_type,uint8_t type_list[10],uint8_t type_num);
/**
 * @brief 设置允许接收的遥控CID类型
 * @note 
 * @param[in] btr_src_t:
 * 
 *
 * @return none
 */
void btr_set_recv_cid_type(btr_cid_type_t type);
btr_cid_type_t btr_get_setting_cid_type();

void btr_parser(uint8_t *data);
/**
 * @brief 注册标准版遥控回调事件
 * @note 
 * @param[in]  btr_cb 回调事件
 *
 * @return none
 */
int btr_regist_data_event(btr_event_cb_t cb);
/**
 * @brief 注册标准版遥控透传数据回调事件
 * @note 
 * @param[in]  btr_cb 回调事件
 *
 * @return none
 */
int btr_regist_tt_data_event(btr_event_cb_t cb);

  /**
 * @brief 获取标准遥控绑定的数量
 * @note 
 * @param[in] none
 * @param[out] none
 *
 * @return 绑定的数量
 */
uint8_t btr_get_bind_count();
  /**
 * @brief 删除标准遥控所有已经绑定的遥控
 * @note 
 * @param[in] none
 * @param[out] none
 *
 * @return none
 */
void btr_clear_all_bind();

#endif