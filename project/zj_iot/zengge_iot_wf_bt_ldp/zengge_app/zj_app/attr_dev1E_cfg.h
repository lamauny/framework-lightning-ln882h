#ifndef VERSION_1E_CONFIG_H
#define VERSION_1E_CONFIG_H

#define PROTOCOL_VERSION                  10

//#define APPCONFIG_OUTPUT_DRV_IIC
#define APPCONFIG_OUTPUT_DRV_PWM

#ifdef APPCONFIG_OUTPUT_DRV_PWM        /* pwm输出的类型，在这里定义功能区分,iic输出类型的，通过读取media来区分iic芯片类型和参数 */
	#define VERSION_TYPE       "PWM"
	#define APPCONFIG_PWM_DEPTH             6000  
#else
  //#define CUSTOMER_DIFF_BP104 
  //#define CUSTOMER_DIFF_BP105 
  //#define CUSTOMER_DIFF_BP108 
  //#define CUSTOMER_DIFF_BP109 
  #define CUSTOMER_DIFF_BP112
 
#if defined(CUSTOMER_DIFF_BP104)
#include "iic_I_cfg/BP/BP104_cfg.h"
#elif defined(CUSTOMER_DIFF_BP105)
#include "iic_I_cfg/BP/BP105_cfg.h"
#elif defined(CUSTOMER_DIFF_BP108)
#include "iic_I_cfg/BP/BP108_cfg.h"
#elif defined(CUSTOMER_DIFF_BP109)
#include "iic_I_cfg/BP/BP109_cfg.h"
#elif defined(CUSTOMER_DIFF_BP112)
#include "iic_I_cfg/BP/BP112_cfg.h"
#else
#error "not define iic current config"
#endif
#endif

#define APPCONFIG_DEFF_MODE               DEF_FACMODE_RGBWC_TEST
#define APPCONFIG_REST_MODE               DEF_RESET_RGBWC_AGING   
#define APPCONFIG_DEF_PWRON_CFG           PWR_ON_STATUS_ALWAY_ON_MEMORY
#define APPCONFIG_SUPPORT_COLOR_RGB                               /* 支持RGB color  */
#define APPCONFIG_SUPPORT_COLOR_CCT                               /* 支持CCT color  */
#define APPCONFIG_DEF_WIRING_TYPE         WIRING_TYPE_NONE   
#define APPCONFIG_RESTORE_COUNT           6
#define APPCONFIG_LONG_PRESS_RESTORE_TIME 8
#define APPCONFIG_RESET_CLR_CFG                 /* 恢复出厂清除参数 */
#define APPCONFIG_SUPPORT_PRODUCT_TEST          /*支持产测*/

#define APPCONFIG_OUTPUT_SORT_CH1        1                       /* 默认通道1颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH2        2                       /* 默认通道2颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH3        3                       /* 默认通道3颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH4        4                       /* 默认通道4颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH5        5                       /* 默认通道5颜色输出排序   */


#endif