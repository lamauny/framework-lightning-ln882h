#ifndef VERSION_21CONFIG_H
#define VERSION_21CONFIG_H

#define PROTOCOL_VERSION                  4                       /* 协议版本号         */

#define VERSION_TYPE                     NULL                    /* 固件后缀id         */
#define APPCONFIG_PWM_DEPTH              6000 
            
#define APPCONFIG_DEFF_MODE               DEF_FACMODE_WARM_TEST    /* 第一次烧录出厂模式 */
#define APPCONFIG_REST_MODE               DEF_RESET_WARM_AGING     /* 重置模式 */
#define APPCONFIG_DEF_PWRON_CFG           PWR_ON_STATUS_ALWAY_ON_MEMORY     /* 默认出厂通电反应设置 */
#define APPCONFIG_SUPPORT_COLOR_WARM           
#define APPCONFIG_DEF_WIRING_TYPE         WIRING_TYPE_NONE     /* 默认接线类型设置 */                         
#define APPCONFIG_RESTORE_COUNT           6      /*  上电多少次恢复出厂设置 */
#define APPCONFIG_LONG_PRESS_RESTORE_TIME 8     /*  长按按键恢复出厂设置时间设置 */
#define APPCONFIG_MAGICHOME_LESS_5CH_CMD        /* magic home：设备类型<5路的兼容 */
#define APPCONFIG_SUPPORT_PUB_BTR               /* 支持公版遥控 */
#define APPCONFIG_SUPPORT_PUB_BTR_TYPE_H2       /* 支持公版H2遥控 */
#define APPCONFIG_NONSUPPORT_BR_DIM_CURVE       /* 取消亮度调光曲线 */
#define APPCONFIG_NONSUPPORT_LOW_LIGHT_LEVEL    /* 取消低亮度处理 */
#define APPCONFIG_RESET_CLR_CFG                 /* 恢复出厂清除参数 */

#define APPCONFIG_OUTPUT_SORT_CH1         2                       /* 默认通道1颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH2         1                       /* 默认通道2颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH3         3                       /* 默认通道3颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH4         4                       /* 默认通道4颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH5         5                       /* 默认通道5颜色输出排序   */

#define APPCONFIG_OUTPUT_DRV_PWM

#endif