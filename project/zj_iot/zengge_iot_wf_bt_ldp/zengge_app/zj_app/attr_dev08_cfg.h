#ifndef VERSION_08CONFIG_H
#define VERSION_08CONFIG_H       

#define PROTOCOL_VERSION                  4 

//#define CUSTOMER_DIFF_3KEY      /* 客户差异化：支持3KEY*/
#define CUSTOMER_DIFF_IR_KEY      /* 客户差异化：支持IR + KEY*/

#ifdef CUSTOMER_DIFF_3KEY
  	#define VERSION_TYPE				"3KEY"			/* 固件后缀id         */
  	#define APPCONFIG_SUPPORT_KEY       KEY_PW_MD_MS    /* 支持3个按键功能  */
	#define APPCONFIG_SUPPORT_IR		IR_44KEY_00FF	/* 支持IR功能  */
	#define APPCONFIG_KEY_PW_GPIO		0 
	#define APPCONFIG_KEY_MD_GPIO		1 
	#define APPCONFIG_KEY_MS_GPIO		2 
#elif defined(CUSTOMER_DIFF_IR_KEY)
	#define VERSION_TYPE				"IRKEY"			/* 固件后缀id         */
  	#define APPCONFIG_SUPPORT_KEY       KEY_PWMD    /* 支持3个按键功能  */
	#define APPCONFIG_SUPPORT_IR		IR_44KEY_00FF	/* 支持IR功能  */
	#define APPCONFIG_KEY_PWMD_GPIO		0
#else
	#define VERSION_TYPE				"KEY"			/* 固件后缀id         */
  	#define APPCONFIG_SUPPORT_KEY       KEY_PWMD        /* 支持1个按键功能  */
	#define APPCONFIG_KEY_PWMD_GPIO		12
#endif


#define APPCONFIG_SUPPORT_REMOTE_B9
#define APPCONFIG_SUPPORT_HW_MIC                       /* 支持硬件麦克风功能  */
#define APPCONFIG_PWM_DEPTH               2000   

#define APPCONFIG_DEFF_MODE               DEF_FACMODE_OTHER_MODE
#define APPCONFIG_REST_MODE               DEF_FACMODE_RGB_JUMP   
#define APPCONFIG_DEF_PWRON_CFG           PWR_ON_STATUS_MEMORY
#define APPCONFIG_SUPPORT_COLOR_RGB                    /* 支持RGB color  */
#define APPCONFIG_DEF_WIRING_TYPE         WIRING_TYPE_NONE   
#define APPCONFIG_RESTORE_COUNT           6
#define APPCONFIG_LONG_PRESS_RESTORE_TIME 8
#define APPCONFIG_MAGICHOME_LESS_5CH_CMD    

#define APPCONFIG_OUTPUT_SORT_CH1        2                       /* 默认通道1颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH2        1                       /* 默认通道2颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH3        3                       /* 默认通道3颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH4        4                       /* 默认通道4颜色输出排序   */
#define APPCONFIG_OUTPUT_SORT_CH5        5                       /* 默认通道5颜色输出排序   */

#define APPCONFIG_OUTPUT_DRV_PWM

#endif