#ifndef ZJ_ADAPT_H
#define ZJ_ADAPT_H

/*适配freeRTOS 头文件*/
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "os_queue.h"
#include "semphr.h"
#include "timers.h"
/*适配对应平台所需的头文件*/
#include "osal/osal.h"
#include "utils/debug/log.h"
#include "ln882h.h"
#include "wifi.h"
#include "wifi_port.h"
#include "netif/ethernetif.h"
#include "wifi_manager.h"
/*适配socket 头文件*/
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#include <lwip/opt.h>
#include <lwip/mem.h>
#include <lwip/icmp.h>
#include <lwip/netif.h>
#include <lwip/inet_chksum.h>
#include <lwip/ip.h>

#include "mbedtls/aes.h"

#if (CONFIG_DEBUG_LOG_LEVEL == 1) /* LOG_I */
  #define zj_printf(fmt, ...)  LOG(LOG_LVL_INFO, fmt, ##__VA_ARGS__)            /* no newline */
  #define os_printf(fmt, ...)  LOG(LOG_LVL_INFO, fmt, ##__VA_ARGS__)            /* no newline */
#elif (CONFIG_DEBUG_LOG_LEVEL == 2) /* LOG_D */
  #define zj_printf(fmt, ...)  LOG(LOG_LVL_DEBUG, fmt, ##__VA_ARGS__)           /* no newline */
  #define os_printf(fmt, ...)  LOG(LOG_LVL_DEBUG, fmt, ##__VA_ARGS__)           /* no newline */
#endif

#ifndef os_printf
#define os_printf(...)
#endif

#ifndef zj_printf
#define zj_printf(...)
#endif

#define zj_dbg_printf(fmt, ...) LOG(LOG_LVL_INFO, fmt, ##__VA_ARGS__)

/*适配线程优先级*/
#define PORT_CONFIG_LIGHT_TASK_PRIO           (configMAX_PRIORITIES) /* true is configMAX_PRIORITIES - 1 */
#define PORT_CONFIG_LIGHT_FUNC_PRIO           (3) // (5)
#define PORT_CONFIG_LIGHT_REMOTE_TASK_PRIO    (2) // (4)
#define PORT_CONFIG_TCP_SERVER_TASK_PRIO      (3) // (12)
#define PORT_CONFIG_TCP_CLIENT_TASK_PRIO      (4) // (13)
#define PORT_CONFIG_TCP_CLIENT_SEND_TASK_PRIO (3) // (10)
#define PORT_CONFIG_UDP_SERVER_TASK_PRIO      (5) // (16)
#define PORT_CONFIG_TIMER_CLOCK_TASK_PRIO     (2) // (2)
#define PORT_CONFIG_AUTO_REPORT_TASK_PRIO     (2) // (3)
#define PORT_CONFIG_FOTA_TASK_PRIO            (3) // (10)
#define PORT_CONFIG_ZG_EVENT_LOOP_TASK_PRIO   (4) // (11)
#define PORT_CONFIG_ZG_WATCHDOG_TASK_PRIO     (1) // (1)
#define PORT_CONFIG_KV_TASK_PRIO              (1) // (1)  
#define PORT_CONFIG_MIC_TASK_PRIO             (3) // (5)  
#define PORT_CONFIG_BT_REMOTE_EVENT_TASK_PRIO (4) // (6)  
#define PORT_CONFIG_KEY_TASK_PRIO             (5) // (15)
#define PORT_CONFIG_BLE_TASK_PRIO             (5) // (15)
#define ZG_DATA_TASK_PRIO                     (3) // (7)
#define PORT_CONFIG_NET_DIAGNOSIS_TASK_PRIO   (4) // (15)

#define PORT_CONFIG_UINT32_T_IS_LUI           /*uint32_t is long unsigned int*/

#define USE_ZG_KV_MODULE_OPTION               0               /*征极基于key值的保存模块*/
#define USE_ZG_KV_LOCK                        0               /*lock*/

extern uint32_t esp_timer_get_time(void);
#define PORT_CONFIG_SYSTEM_TIME               OS_GetTime() / 1000                          /* 适配系统时间 */
#define PROT_CONFIG_KEY_FLASH_SIZE            (4096)                                       /* 适配保存密钥的flash区域大小 */

#define PROT_CONFIG_KEY_FLASH_OFFSET          0x001F8000                                   /* 适配密钥在对应flash地址的起始值*/
#define PROT_CONFIG_KEY_FLASH_ADDR            0x0000 + PROT_CONFIG_KEY_FLASH_OFFSET        /* 适配保存密钥的flash地址，4K */
#define PORT_CONFIG_CIPHERTEXT_FLASH_ADDR     0x001F9000                                   /* 适配保存密文的flash地址，4K */
#define PORT_CONFIG_IIC_FLASH_ADDR            0x001FF000                                   /* Murphy TODOs */
#define PORT_CONFIG_USER_DATA_FLASH_ADDR      0x001FA000                                   /* 适配保存用户数据的flash地址 */
#define PORT_CONFIG_USER_DATA_SIZE            16*1024                                      /* 适配保存用户数据的flash大小 16K */
#define PORT_BTR_NEW_SCENE_ADDR               0x001FE000                                   /* 适配场景功能，4K */

#define PORT_CONFIG_OTA_IMAGE_NAME            "esp_ota.bin.xz.packed"                      /* 适配ota文件名 */


#define PORT_CONFIG_PWM_RED_CHANNEL           3                                 /*适配pwm red channel*/
#define PORT_CONFIG_PWM_GREEN_CHANNEL         2                                 /*适配pwm green channel*/
#define PORT_CONFIG_PWM_BLUE_CHANNEL          1                                 /*适配pwm blue channel*/
#define PORT_CONFIG_PWM_WARM_CHANNEL          4
#define PORT_CONFIG_PWM_COLD_CHANNEL          0

#define PORT_CONFIG_IIC_SDA_PIN2             
#define PORT_CONFIG_IIC_SCL_PIN2            

#define PORT_CONFIG_IIC_SDA_PIN               8
#define PORT_CONFIG_IIC_SCL_PIN               9

/*IIC电流适配 */
#define IIC_RGB_7W_mA                         RGB_19mA
#define IIC_WY_7W_mA                          WY_67mA

#define IIC_RGB_EH9WL_mA                      RGB_14mA
#define IIC_WY_EH9WL_mA                       WY_59mA

#define IIC_RGB_EH9W_mA                       RGB_9mA
#define IIC_WY_EH9W_mA                        WY_30mA

#define IIC_RGB_9W_mA                         RGB_21_4mA
#define IIC_WY_9W_mA                          WY_90_0mA

#define IIC_RGB_CANDLE_mA                     RGB_19mA
#define IIC_WY_CANDLE_mA                      WY_20mA

/*  插座适配IO */
#define PORT_CONFIG_PLUG_OUT_GPIO                     
#define PORT_CONFIG_PLUG_LED_RED             
#define PORT_CONFIG_PLUG_LED_BLUE            
#define PORT_CONFIG_PLUG_KEY_GPIO           
#define PORT_CONFIG_PLUG_SWITCH_GPIO         
#define PORT_CONFIG_USB1_GPIO                
#define PORT_CONFIG_USB2_GPIO                

/*  落地灯适配IO */
#define PORT_CONFIG_LIGHT_KEY_GPIO            26  

/*  圣诞灯适配IO */
#define PORT_CONFIG_XMAS_KEY_GPIO             

/*  高压模块适配IO */
#define PORT_CONFIG_HV_WAVE_GPIO             

/* plant light IO */
#define PORT_CONFIG_PLANT_KEY1_GPIO            
#define PORT_CONFIG_PLANT_KEY2_GPIO            
#define PORT_CONFIG_PLANT_KEY3_GPIO                   
#define PORT_CONFIG_PLANT_KEY4_GPIO                  
#define PORT_CONFIG_PLANT_KEY5_GPIO           

#define USE_BT_CONFIG_NET                      1
#define IS_LN_PLATFORM

#endif
