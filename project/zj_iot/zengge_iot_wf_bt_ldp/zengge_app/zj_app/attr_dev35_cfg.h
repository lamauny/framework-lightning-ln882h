#ifndef VERSION_35_CONFIG_H
#define VERSION_35_CONFIG_H

#define PROTOCOL_VERSION                  10

#define APPCONFIG_OUTPUT_DRV_IIC
// #define APPCONFIG_OUTPUT_DRV_PWM

#ifdef APPCONFIG_OUTPUT_DRV_PWM        /* pwm输出的类型，在这里定义功能区分,iic输出类型的，通过读取media来区分iic芯片类型和参数 */
  //#define CUSTOMER_DIFF_610HZ      /* 客户差异化：pwm定制为611hz */
  #define CUSTOMER_DIFF_UFO      /* 客户差异化：pwm定制为1khz 支持KEY*/
  //#define CUSTOMER_DIFF_LWS_AC      /* 客户差异化：支持AC检测 支持独立小夜灯*/

	#ifdef CUSTOMER_DIFF_UFO
	  #define VERSION_TYPE      "UFO"                     /* 固件后缀id         */
	  #define APPCONFIG_SUPPORT_KEY             KEY_PWMD  /* 支持按键功能  */
		#define APPCONFIG_KEY_PWMD_GPIO		      12
    #define APPCONFIG_PWM_DEPTH            1000
	#elif defined(CUSTOMER_DIFF_610HZ)
	  #define VERSION_TYPE       "611HZ"
	  #define APPCONFIG_PWM_DEPTH             611
	#elif defined(CUSTOMER_DIFF_LWS_AC)
	  #define VERSION_TYPE			"AC"
	  #define CUSTOMER_NAME			"LWS"    	  /* 客户标识，LWS代表力瓦士 */
	  #define APPCONFIG_SUPPORT_NIGHT_LAMP
	  #define APPCONFIG_SUPPORT_AC_MONITOR
	  #define APPCONFIG_PWM_DEPTH            2000
	#else
	  #define VERSION_TYPE       "PWM"
	  #define APPCONFIG_PWM_DEPTH             6000
	#endif
#else
  #define LIGHT_CORE_DL_TIME      10
  //#define CUSTOMER_DIFF_BP101
  #define CUSTOMER_DIFF_BP102
  //#define CUSTOMER_DIFF_BP103
  //#define CUSTOMER_DIFF_BP106
  //#define CUSTOMER_DIFF_BP107
  //#define CUSTOMER_DIFF_BP110
  //#define CUSTOMER_DIFF_BP111
  //#define CUSTOMER_DIFF_BP113
  // #define CUSTOMER_DIFF_BP114
  //#define CUSTOMER_DIFF_EJ101

  //#define CUSTOMER_DIFF_EH101
  //#define CUSTOMER_DIFF_EH102
  //#define CUSTOMER_DIFF_EH103
  //#define CUSTOMER_DIFF_EH104

  //#define CUSTOMER_DIFF_EGH101
  //#define CUSTOMER_DIFF_EGH102
  //#define CUSTOMER_DIFF_EGH103
  //#define CUSTOMER_DIFF_EGH104
  //#define CUSTOMER_DIFF_EGH105
  //#define CUSTOMER_DIFF_EGH106
  //#define CUSTOMER_DIFF_EGH107
  //#define CUSTOMER_DIFF_EGH108

  //#define CUSTOMER_DIFF_IH9W
  //#define CUSTOMER_DIFF_IH9WL

  #ifdef CUSTOMER_DIFF_BP101
  #include "iic_I_cfg/BP/BP101_cfg.h"
  #define APP_CONFIG_RESET_METHOD2           /* 第一次配网前，开关3次恢复出厂设置；第一次配网后，开关APP_CONFIG_RESET_COUNT次恢复出厂设置  */
  #elif defined(CUSTOMER_DIFF_BP102)
  #define APP_CONFIG_RESET_METHOD2           /* 第一次配网前，开关3次恢复出厂设置；第一次配网后，开关APP_CONFIG_RESET_COUNT次恢复出厂设置  */
  #include "iic_I_cfg/BP/BP102_cfg.h"
  #elif defined(CUSTOMER_DIFF_BP103)
  #include "iic_I_cfg/BP/BP103_cfg.h"
  #elif defined(CUSTOMER_DIFF_BP106)
  #include "iic_I_cfg/BP/BP106_cfg.h"
  #elif defined(CUSTOMER_DIFF_BP107)
  #include "iic_I_cfg/BP/BP107_cfg.h"
  #elif defined(CUSTOMER_DIFF_BP110)
  #include "iic_I_cfg/BP/BP110_cfg.h"
  #elif defined(CUSTOMER_DIFF_BP111)
  #include "iic_I_cfg/BP/BP111_cfg.h"
  #elif defined(CUSTOMER_DIFF_BP113)
  #include "iic_I_cfg/BP/BP113_cfg.h"
  #define APPCONFIG_SUPPORT_C_REMOTE_0X04_0x0A
  #elif defined(CUSTOMER_DIFF_BP114)
  #include "iic_I_cfg/BP/BP114_cfg.h"
  #define APPCONFIG_SUPPORT_C_REMOTE_0X04_0x0A
  #elif defined(CUSTOMER_DIFF_EJ101)
  #include "iic_I_cfg/EJ/EJ101_cfg.h"
  #elif defined(CUSTOMER_DIFF_EH101)
  #include "iic_I_cfg/EH/EH101_cfg.h"
  #elif defined(CUSTOMER_DIFF_EH102)
  #include "iic_I_cfg/EH/EH102_cfg.h"
  #elif defined(CUSTOMER_DIFF_EH103)
  #include "iic_I_cfg/EH/EH103_cfg.h"
  #elif defined(CUSTOMER_DIFF_EH104)
  #include "iic_I_cfg/EH/EH104_cfg.h"
  #elif defined(CUSTOMER_DIFF_EGH101)
  #include "iic_I_cfg/EGH/EGH101_cfg.h"
  #elif defined(CUSTOMER_DIFF_EGH102)
  #include "iic_I_cfg/EGH/EGH102_cfg.h"
  #elif defined(CUSTOMER_DIFF_EGH103)
  #include "iic_I_cfg/EGH/EGH103_cfg.h"
  #elif defined(CUSTOMER_DIFF_EGH104)
  #include "iic_I_cfg/EGH/EGH104_cfg.h"
  #elif defined(CUSTOMER_DIFF_EGH105)
  #include "iic_I_cfg/EGH/EGH105_cfg.h"
  #elif defined(CUSTOMER_DIFF_EGH106)
  #include "iic_I_cfg/EGH/EGH106_cfg.h"
  #elif defined(CUSTOMER_DIFF_EGH107)
  #include "iic_I_cfg/EGH/EGH107_cfg.h"
  #elif defined(CUSTOMER_DIFF_EGH108)
  #include "iic_I_cfg/EGH/EGH108_cfg.h"
  #elif defined(CUSTOMER_DIFF_IH9W)
  #include "iic_I_cfg/IH/IH9W_cfg.h"
  #define VERSION_TYPE       "IH9W"
  #elif defined(CUSTOMER_DIFF_IH9WL)
  #include "iic_I_cfg/IH/IH9WL_cfg.h"
  #define VERSION_TYPE       "IH9WL"
  #else
  #error "not define iic current config"
  #endif

#endif

#define APPCONFIG_DEFF_MODE               DEF_FACMODE_RGBWC_TEST  /* 第一次出厂默认模式 */
#define APPCONFIG_REST_MODE               DEF_RESET_RGBWC_AGING   /* 恢复出厂后模式设置 */
#define APPCONFIG_DEF_PWRON_CFG           PWR_ON_STATUS_ALWAY_ON_MEMORY /* 默认出厂通电反应设置 */
#define APPCONFIG_SUPPORT_COLOR_RGB                               /* 支持RGB color  */
#define APPCONFIG_SUPPORT_COLOR_CCT                               /* 支持CCT color  */
#define APPCONFIG_DEF_WIRING_TYPE         WIRING_TYPE_NONE        /* 默认接线类型设置 */
#define APPCONFIG_RESTORE_COUNT           6     /*  上电多少次恢复出厂设置 */
#define APPCONFIG_LONG_PRESS_RESTORE_TIME 8     /*  长按按键恢复出厂设置时间设置 */
#define APPCONFIG_RESET_CLR_CFG                 /* 恢复出厂清除参数 */

#define APPCONFIG_OUTPUT_SORT_CH1        1                       /* 默认通道1颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH2        2                       /* 默认通道2颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH3        3                       /* 默认通道3颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH4        4                       /* 默认通道4颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH5        5                       /* 默认通道5颜色输出排序   */


#endif