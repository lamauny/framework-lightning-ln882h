#ifndef VERSION_35_CONFIG_H
#define VERSION_35_CONFIG_H

#define PROTOCOL_VERSION                  3
#define VERSION_TYPE                    NULL

#ifdef CUSTOMER_DIFF_UFO
  #define VERSION_TYPE      "UFO"                     /* 固件后缀id         */
  #define APPCONFIG_SUPPORT_KEY           KEY_PWMD    /* 支持按键功能  */
  #define APPCONFIG_PWM_DEPTH             1000
#else
  #define VERSION_TYPE                    NULL
  #define APPCONFIG_PWM_DEPTH             2000  
#endif

#define APPCONFIG_DEFF_MODE               DEF_FACMODE_RGBWC_JUMP
#define APPCONFIG_REST_MODE               DEF_FACMODE_RGBWC_JUMP   
#define APPCONFIG_DEF_PWRON_CFG           PWR_ON_STATUS_MEMORY
#define APPCONFIG_SUPPORT_COLOR_RGB                               /* 支持RGB color  */
#define APPCONFIG_SUPPORT_COLOR_CCT                               /* 支持CCT color  */
#define APPCONFIG_DEF_WIRING_TYPE         WIRING_TYPE_RGBWC   
#define APPCONFIG_RESTORE_COUNT           6
#define APPCONFIG_ENABLE_CFG_RGBWC_BOTH   1
#define APPCONFIG_LONG_PRESS_RESTORE_TIME 8

#define APPCONFIG_OUTPUT_SORT_CH1        1                       /* 默认通道1颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH2        2                       /* 默认通道2颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH3        3                       /* 默认通道3颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH4        4                       /* 默认通道4颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH5        5                       /* 默认通道5颜色输出排序   */

#define APPCONFIG_OUTPUT_DRV_PWM

#endif