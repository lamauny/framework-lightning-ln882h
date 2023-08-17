#ifndef VERSION_CONFIG_H
#define VERSION_CONFIG_H


/*------   Version   ------*/
#define APP_VERSION_MAJOR    4    /* for breaking interface changes  */
#define APP_VERSION_MINOR    10    /* for new (non-breaking) interface capabilities */
#define APP_VERSION_REVISION 0    /* for tweaks, bug-fixes, or development */
#define APP_BUILD_NUMBER     0

#define APP_VERSION_NUMBER (APP_VERSION_MAJOR *100*100 + APP_VERSION_MINOR *100 + APP_VERSION_REVISION)

#define APP_LIB_VERSION APP_VERSION_MAJOR.APP_VERSION_MINOR.APP_VERSION_REVISION(APP_BUILD_NUMBER)
#define APP_QUOTE(str) #str
#define APP_EXPAND_AND_QUOTE(str) APP_QUOTE(str)
#define APP_VERSION_STRING APP_EXPAND_AND_QUOTE(APP_LIB_VERSION)  /* requires v1.7.3+ */


/* 设备类型定义 */
//#define DEVICE_TYPE        0x35    //rgbwc bulb
//#define DEVICE_TYPE        0x1E    //ceiling light
#define DEVICE_TYPE        0x0E    //floor light

//#define DEVICE_TYPE        0x29    //rgbwc mirror bulb
//#define DEVICE_TYPE        0x33    //rgb ir
//#define DEVICE_TYPE        0x08    //rgb mic
//#define DEVICE_TYPE        0x62    //cct controller
//#define DEVICE_TYPE        0x41    //warm controller
//#define DEVICE_TYPE        0x21    //warm bulb
//#define DEVICE_TYPE        0x06    //rgbw controller
//#define DEVICE_TYPE        0x48    //rgbw controller and mic
//#define DEVICE_TYPE        0x07    //rgbwc controller
//#define DEVICE_TYPE        0x52    //cct bulb

 /*通过编译脚本区分不同的编译平台*/
#ifdef IS_LN_PLATFORM
    #define BLE_CHIP_ID 0x5D  /* This configuration is provided by Zhengji */
    #define CHIP_MANUFACTURER "LN"
#elif defined(IS_BK_PLATFORM)  /*bkmake博通平台*/
  #ifdef CONFIG_MAC_COMPATIBILITY
     #define CHIP_MANUFACTURER      "BK"
  #else
     #define CHIP_MANUFACTURER      "BK2"
  #endif /* CONFIG_MAC_COMPATIBILITY */
  #define BLE_CHIP_ID             0x50
#else  /*博流平台*/

#ifndef APPCONFIG_SURPLIFE_CMD
  #ifdef APPCONFIG_MAGICHOME_CMD  
      #define BLE_CHIP_ID     0x51  /* 只支持magichome */
  #endif
#endif
#ifndef BLE_CHIP_ID
#define BLE_CHIP_ID     0x52
#endif

	#ifdef USE_1M_PARTITION   /*博流1M FLASH 只支持surplife app*/
		#ifdef APPCONFIG_SURPLIFE_CMD
			#define CHIP_MANUFACTURER     "B1M"
		#else                   /*博流1M FLASH 只支持magichome app*/
			#define CHIP_MANUFACTURER     "B1M2"
		#endif
	#else   /*博流1M FLASH*/
		#define CHIP_MANUFACTURER     "BL"
	#endif
#endif

#define FRIMWARE_VERSION       218    /* 固件版本序号 */
#define MID_VERSION            12      /* MID 版本*/
#define CUSTOMER_NAME          "ZG"    /* 客户标识，ZG代表标准固件 */     

#if (DEVICE_TYPE == 0x33)                                      
#include "attr_dev33_cfg.h"
#elif (DEVICE_TYPE == 0x06)          
#include "attr_dev06_cfg.h"
#elif (DEVICE_TYPE == 0x48)          
#include "attr_dev48_cfg.h"
#elif (DEVICE_TYPE == 0x08)          
#include "attr_dev08_cfg.h"
#elif (DEVICE_TYPE == 0x62)          
#include "attr_dev62_cfg.h"
#elif (DEVICE_TYPE == 0x52)          
#include "attr_dev52_cfg.h"
#elif (DEVICE_TYPE == 0x41)          
#include "attr_dev41_cfg.h"
#elif (DEVICE_TYPE == 0x21)          
#include "attr_dev21_cfg.h"
#elif (DEVICE_TYPE == 0x29)          
#include "attr_dev29_cfg.h"
#elif (DEVICE_TYPE == 0x07)          
#include "attr_dev07_cfg.h"
#elif (DEVICE_TYPE == 0x1E)          
#include "attr_dev1E_cfg.h"
#elif (DEVICE_TYPE == 0x0E)          
#include "attr_dev0E_cfg.h"
#else //device type 0x35
#include "attr_dev35_cfg.h"
#endif


#if defined(APPCONFIG_OUTPUT_DRV_IIC) && defined(APPCONFIG_OUTPUT_DRV_PWM)
	#error OUTPUT_TYPE iic & pwm Cannot be set at the same time
#endif

/* 按键模式定义 */
/*//////////////////////////////////////
PW:代表开关
MD:代表模式
MS:音乐
LPN:长按恢复出厂N秒
LPD:长按调光
BR:亮度
//////////////////////////////////////*/
#define KEY_PWMD				1
#define KEY_LPDMD				2
#define KEY_PW_MD_MS			3

/* 红外模式定义 */
/*//////////////////////////////////////

//////////////////////////////////////*/
#define IR_24KEY_00FF			1
#define IR_44KEY_00FF			2


/* 第一次烧录出厂模式定义 */
#define DEF_FACMODE_OTHER_MODE    0
#define DEF_FACMODE_RGB_JUMP      1
#define DEF_FACMODE_RGBW_JUMP     2
#define DEF_FACMODE_RGBWC_JUMP    4
#define DEF_FACMODE_RGBWC_TEST    5
#define DEF_FACMODE_CCT_TEST      6
#define DEF_FACMODE_WARM_TEST     7
/* reset模式定义 */
#define DEF_RESET_RGBWC_AGING     8
#define DEF_RESET_CCT_AGING       9
#define DEF_RESET_WARM_AGING      10


#define WIRING_TYPE_NONE           0
#define WIRING_TYPE_RGB            1
#define WIRING_TYPE_RGBW           2
#define WIRING_TYPE_RGBW_BOTH      3
#define WIRING_TYPE_RGBWC          4
#define WIRING_TYPE_RGBWC_BOTH     5
#define WIRING_TYPE_W              6
#define WIRING_TYPE_C              7
#define WIRING_TYPE_WC             8


#define USE_BT_REMOTE          1          /* 是否启动蓝牙遥控 */

#endif