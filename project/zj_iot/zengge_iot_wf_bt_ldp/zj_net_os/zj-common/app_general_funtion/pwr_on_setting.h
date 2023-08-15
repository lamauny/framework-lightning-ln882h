#ifndef LHT_PWR_ON_SETTING__H
#define LHT_PWR_ON_SETTING__H

typedef enum{
   
   PWR_ON_STATUS_DEFAULT,
   PWR_ON_STATUS_MEMORY,
   PWR_ON_STATUS_CUSTOM,
   PWR_ON_STATUS_ALWAY_ON_MEMORY,
   PWR_ON_STATUS_ALWAY_OFF
}pwr_on_status_t;

typedef struct{
   
   pwr_on_status_t status;
   unsigned char two_force_on_flag;//连续两次上电可开灯
   unsigned char custom_color_type;
   unsigned char execution_type;      /*执行类型*/ 
   unsigned int h;
   unsigned char s;
   unsigned char cct;
   unsigned char rgb_br;
   unsigned char cct_br;  
}pwr_on_setting_t;


typedef int (*pwr_on_setting_cb_t)(pwr_on_setting_t *setting);

/** 
 * @brief 通电反应-设置出厂默认的通电反应状态
 * @note  
 * @param[in] status
 *  PWR_ON_STATUS_DEFAULT
 *  PWR_ON_STATUS_MEMORY
 *  PWR_ON_STATUS_CUSTOM
 *  PWR_ON_STATUS_ALWAY_ON_MEMORY
  * PWR_ON_STATUS_ALWAY_OFF
 * @return none
 */
void power_on_set_status_default(pwr_on_status_t status);
/** 
 * @brief 设置出厂默认的连续两次上电可开灯标志
 * @note  
 * @param[in] status
 *  1: on
 *  2:off
 * @return none
 */
void power_on_set_twice_on_default(bool status);
/** 
 * @brief 通电反应-配置数据
 * @note  
 * @param[in] 配置数据
 * @return none
 */
void power_on_set_config(unsigned char *setting_data);
/** 
 * @brief 通电反应-获取配置数据
 * @note  
 * @param[out] 配置数据
 * @return none
 */
void power_on_get_config(unsigned char *setting);
/** 
 * @brief 通电反应-初始化
 * @note  
 * @param[in] 通电反应的回调函数
 * @return 0 - success -1 : fail
 */
int dev_power_on_init(pwr_on_setting_cb_t cb);
/** 
 * @brief 通电反应-清除
 * @note  
 * @param[in] none
 * @return none
 */
void power_on_clear();
#endif