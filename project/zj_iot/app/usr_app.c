#include "osal/osal.h"
#include "utils/debug/log.h"
#include "utils/debug/ln_assert.h"
#include "utils/system_parameter.h"
#include "utils/ln_psk_calc.h"
#include "utils/power_mgmt/ln_pm.h"
#include "utils/sysparam_factory_setting.h"
#include "wifi.h"
#include "wifi_port.h"
#include "netif/ethernetif.h"
#include "wifi_manager.h"
#include "lwip/tcpip.h"
#include "drv_adc_measure.h"
#include "hal/hal_adc.h"
#include "ln_nvds.h"
#include "ln_wifi_err.h"
#include "ln_misc.h"
#include "ln882h.h"
#include "usr_app.h"

static OS_Thread_t g_zj_main_thread;
static OS_Thread_t g_temp_thread;

// #define ZJ_TEST_ONLY

#define USR_APP_TASK_STACK_SIZE   6*1024 //Byte
#define TEMP_APP_TASK_STACK_SIZE  2*1024 //Byte

static void temp_app_task_entry(void *params);

extern void ZG_light_system_start(void);
extern int aes_key_init();
extern void zj_adapter_init();
extern void net_type_init();
extern void zj_watchdog_start();

static void zj_main_entry(void *params)
{
    ln_pm_sleep_mode_set(ACTIVE);
#ifndef ZJ_TEST_ONLY
    ZG_light_system_start();/*灯光初始化 */
    aes_key_init();
    zj_adapter_init();
    net_type_init(); /* 网络类型初始化 */
    zj_watchdog_start();
#else
    extern void zj_ln_adapt_test(void);
    zj_ln_adapt_test();
#endif
    OS_ThreadDelete(NULL);
}

static void temp_app_task_entry(void *params)
{
    uint8_t cnt = 0;
    LN_UNUSED(params);

    OS_MsDelay(5000);

    /* print sdk version */
    {
        LOG(LOG_LVL_INFO, "LN882H SDK Ver: %s [build time:%s][0x%08x]\r\n",
                LN882H_SDK_VERSION_STRING, LN882H_SDK_BUILD_DATE_TIME, LN882H_SDK_VERSION);
    }

    while (1)
    {
        cnt++;
        if ((cnt % 10) == 0) {
            LOG(LOG_LVL_INFO, "Total:%d; Free:%ld;\r\n", 
                    OS_HeapSizeGet(), OS_GetFreeHeapSize());
        }
        OS_MsDelay(1000);
    }
}

void creat_usr_app_task(void)
{
    if(OS_OK != OS_ThreadCreate(
        &g_zj_main_thread, "zj main", zj_main_entry,
        NULL, OS_PRIORITY_HIGH, USR_APP_TASK_STACK_SIZE)) {
        LN_ASSERT(1);
    }

    // ble_creat_usr_app_task();

#if  WIFI_TEMP_CALIBRATE
    if(OS_OK != OS_ThreadCreate(&g_temp_thread, "TempAPP", temp_app_task_entry, NULL, OS_PRIORITY_BELOW_NORMAL, TEMP_APP_TASK_STACK_SIZE)) {
        LN_ASSERT(1);
    }
#endif
}
