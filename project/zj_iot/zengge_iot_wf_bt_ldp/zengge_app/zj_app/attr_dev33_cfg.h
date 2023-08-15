#ifndef VERSION_33CONFIG_H
#define VERSION_33CONFIG_H


#define CUSTOMER_DIFF_UFO      /* 客户差异化：pwm定制为1khz 支持KEY*/

#ifdef CUSTOMER_DIFF_UFO
  #define VERSION_TYPE                    "UFO"                    /* 固件后缀id         */
  #define APPCONFIG_SUPPORT_KEY           KEY_PWMD                 /* 支持按键功能  */
  #define APPCONFIG_KEY_PWMD_GPIO		      12
  #define APPCONFIG_PWM_DEPTH               1000 
#else
  #define VERSION_TYPE                     NULL                    /* 固件后缀id         */
  #define APPCONFIG_SUPPORT_IR             IR_24KEY_00FF           /* 支持IR功能  */
  #define APPCONFIG_PWM_DEPTH               2000 
#endif

#define PROTOCOL_VERSION                 12                       /* 协议版本号         */

#define APPCONFIG_SUPPORT_REMOTE_B9
#define APPCONFIG_DEFF_MODE               DEF_FACMODE_RGB_JUMP    /* 第一次烧录出厂模式 */
#define APPCONFIG_REST_MODE               DEF_FACMODE_RGB_JUMP    /* 重置模式 */
#define APPCONFIG_DEF_PWRON_CFG           PWR_ON_STATUS_MEMORY    /* 默认出厂通电反应设置 */
#define APPCONFIG_SUPPORT_COLOR_RGB                               /* 支持RGB color  */
#define APPCONFIG_DEF_WIRING_TYPE         WIRING_TYPE_NONE        /* 默认接线类型设置 */         
#define APPCONFIG_RESTORE_COUNT           6                       /*  上电多少次恢复出厂设置 */
#define APPCONFIG_LONG_PRESS_RESTORE_TIME 8                       /*  长按按键恢复出厂设置时间设置 */
#define APPCONFIG_MAGICHOME_LESS_5CH_CMD                          /* magic home：设备类型<5路的兼容 */

#define APPCONFIG_OUTPUT_SORT_CH1        2                       /* 默认通道1颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH2        1                       /* 默认通道2颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH3        3                       /* 默认通道3颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH4        4                       /* 默认通道4颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH5        5                       /* 默认通道5颜色输出排序   */

#define APPCONFIG_OUTPUT_DRV_PWM

#endif