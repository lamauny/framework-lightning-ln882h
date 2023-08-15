#ifndef VERSION_0E_CONFIG_H
#define VERSION_0E_CONFIG_H


#define CUSTOMER_DIFF_V2_VERSION      /* 客户差异化：换转RGB排序*/


#ifdef CUSTOMER_DIFF_V2_VERSION
	#define VERSION_TYPE				"PWMV2"			/* 固件后缀id         */
	#define APPCONFIG_SUPPORT_A_REMOTE_0X04           	/*支持遥控 0X04*/
	#define APPCONFIG_DEF_PWRON_CFG           PWR_ON_STATUS_MEMORY    /* 默认出厂通电反应设置 */
	#define APPCONFIG_OUTPUT_SORT_CH1        1                       /* 默认通道1颜色输出排序   */
	#define APPCONFIG_OUTPUT_SORT_CH2        3                       /* 默认通道2颜色输出排序   */
	#define APPCONFIG_OUTPUT_SORT_CH3        2                       /* 默认通道3颜色输出排序   */
	#define APPCONFIG_OUTPUT_SORT_CH4        4                       /* 默认通道4颜色输出排序   */
	#define APPCONFIG_OUTPUT_SORT_CH5        5                       /* 默认通道5颜色输出排序   */
#else
	#define VERSION_TYPE				"PWM"			/* 固件后缀id         */
	#define APPCONFIG_DEF_PWRON_CFG		PWR_ON_STATUS_ALWAY_ON_MEMORY    /* 默认出厂通电反应设置 */
	#define APPCONFIG_OUTPUT_SORT_CH1        1                       /* 默认通道1颜色输出排序   */
	#define APPCONFIG_OUTPUT_SORT_CH2        2                       /* 默认通道2颜色输出排序   */
	#define APPCONFIG_OUTPUT_SORT_CH3        3                       /* 默认通道3颜色输出排序   */
	#define APPCONFIG_OUTPUT_SORT_CH4        4                       /* 默认通道4颜色输出排序   */
	#define APPCONFIG_OUTPUT_SORT_CH5        5                       /* 默认通道5颜色输出排序   */
#endif


#define PROTOCOL_VERSION                  10

#define APPCONFIG_PWM_DEPTH               2000

#define APPCONFIG_DEFF_MODE               DEF_FACMODE_RGBWC_TEST
#define APPCONFIG_REST_MODE               DEF_RESET_RGBWC_AGING   
#define APPCONFIG_SUPPORT_COLOR_RGB                               /* 支持RGB color  */
#define APPCONFIG_SUPPORT_COLOR_CCT                               /* 支持CCT color  */
#define APPCONFIG_DEF_WIRING_TYPE         WIRING_TYPE_NONE   
#define APPCONFIG_RESTORE_COUNT           6
#define APPCONFIG_LONG_PRESS_RESTORE_TIME 20
#define APPCONFIG_SUPPORT_KEY             KEY_LPDMD     /* 按键类型：长按调光 */
#define APPCONFIG_NONSUPPORT_BR_DIM_CURVE       /* 取消亮度调光曲线 */
#define APPCONFIG_NONSUPPORT_LOW_LIGHT_LEVEL    /* 取消低亮度处理 */
#define APPCONFIG_RESET_CLR_CFG                 /* 恢复出厂清除参数 */
#define APPCONFIG_SUPPORT_PRODUCT_TEST          /*支持产测*/


#define APPCONFIG_OUTPUT_DRV_PWM

#endif