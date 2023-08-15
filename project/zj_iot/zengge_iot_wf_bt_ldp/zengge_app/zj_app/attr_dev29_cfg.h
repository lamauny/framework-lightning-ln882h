#ifndef VERSION_29CONFIG_H
#define VERSION_29CONFIG_H

#define PROTOCOL_VERSION                  10

#define APPCONFIG_PWM_DEPTH             16000
#define VERSION_TYPE       NULL

#define APPCONFIG_SUPPORT_HW_MIC       /* 支持硬件麦克风功能  */

#define APPCONFIG_DEFF_MODE               DEF_FACMODE_OTHER_MODE
#define APPCONFIG_REST_MODE               DEF_RESET_RGBWC_AGING   
#define APPCONFIG_DEF_PWRON_CFG           PWR_ON_STATUS_ALWAY_OFF
#define APPCONFIG_SUPPORT_COLOR_RGB                               /* 支持RGB color  */
#define APPCONFIG_SUPPORT_COLOR_CCT                               /* 支持CCT color  */
#define APPCONFIG_DEF_WIRING_TYPE         WIRING_TYPE_NONE   
#define APPCONFIG_RESTORE_COUNT           4

#define APPCONFIG_OUTPUT_SORT_CH1        1                       /* 默认通道1颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH2        2                       /* 默认通道2颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH3        3                       /* 默认通道3颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH4        4                       /* 默认通道4颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH5        5                       /* 默认通道5颜色输出排序   */

#define APPCONFIG_OUTPUT_DRV_PWM

#endif