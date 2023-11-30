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
#include "easyflash.h"

#include "zj_adapt_config.h"

static OS_Thread_t g_zj_main_thread;
static OS_Thread_t g_temp_thread;

//#define ZJ_TEST_ONLY
// #define ZJ_APPKEY_TEST
#define WIFI_TEMP_CALIBRATE 1

#define USR_APP_TASK_STACK_SIZE   6*1024 //Byte
#define TEMP_APP_TASK_STACK_SIZE  2*1024 //Byte

#ifdef ZJ_APPKEY_TEST
/* Murphy Used!!! */
const static char *s_zj_appkey = "{\"uuid\": \"c3884e9c8c10499e975ce22a678b8303\", \"aes\": \"AA1C24E979\", \"aesVer\": 3, \"extend\": {\"appKey\": \"ZG002\"}}";

// /* 1 */
// const static char *s_zj_appkey = "{\"uuid\": \"2108d0a383e740968c5f85ca26c9d5cd\", \"aes\": \"BAD64A735E\", \"aesVer\": 3, \"extend\": {\"appKey\": \"ZG002\"}}";

// /* 2 */
// const static char *s_zj_appkey = "{\"uuid\": \"398648779187408a992fa097a57851eb\", \"aes\": \"4F3E3C269F\", \"aesVer\": 3, \"extend\": {\"appKey\": \"ZG002\"}}";

// /* 3 */
// const static char *s_zj_appkey = "{\"uuid\": \"b9be4acd263647109174581ce927fa7e\", \"aes\": \"78155A6178\", \"aesVer\": 3, \"extend\": {\"appKey\": \"ZG002\"}}";

// /* 4 */
// const static char *s_zj_appkey = "{\"uuid\": \"be96a12a6d1642c9962e7f0dc2b8fd81\", \"aes\": \"F692C36644\", \"aesVer\": 3, \"extend\": {\"appKey\": \"ZG002\"}}";

// /* 5 */
// const static char *s_zj_appkey = "{\"uuid\": \"f331dc81fb5043709e9bddb7f040704f\", \"aes\": \"3CF0245680\", \"aesVer\": 3, \"extend\": {\"appKey\": \"ZG002\"}}";

// /* 6 */
// const static char *s_zj_appkey = "{\"uuid\": \"fb1759a22fc1404cb2313d2f5c7db2f6\", \"aes\": \"5D156841B0\", \"aesVer\": 3, \"extend\": {\"appKey\": \"ZG002\"}}";

// /* 7 */
// const static char *s_zj_appkey = "{\"uuid\": \"43b63287fb7140009a0e0470ba38e4d4\", \"aes\": \"E3BABBC78E\", \"aesVer\": 3, \"extend\": {\"appKey\": \"ZG002\"}}";

// /* 8 */
// const static char *s_zj_appkey = "{\"uuid\": \"bdd8d18d4dcb40759649d970e67c0688\", \"aes\": \"CE76090CDC\", \"aesVer\": 3, \"extend\": {\"appKey\": \"ZG002\"}}";

// /* 9 */
// const static char *s_zj_appkey = "{\"uuid\": \"dc47a39322db483891c50a4ac54c45db\", \"aes\": \"19B4658C41\", \"aesVer\": 3, \"extend\": {\"appKey\": \"ZG002\"}}";

// /* 10 */
// const static char *s_zj_appkey = "{\"uuid\": \"641c9497d4bd41a59c4f28b5c66967f4\", \"aes\": \"5176D8498A\", \"aesVer\": 3, \"extend\": {\"appKey\": \"ZG002\"}}";
#endif

static void temp_app_task_entry(void *params);

extern void ZG_light_system_start(void);
extern int aes_key_init();
extern void zj_adapter_init();
extern void net_type_init();
extern void zj_watchdog_start();

static uint8_t test_buf[256];

static void zj_main_entry(void *params)
{
    ln_pm_sleep_mode_set(ACTIVE);

#ifdef ZJ_APPKEY_TEST
    hal_flash_erase(PROT_CONFIG_KEY_FLASH_ADDR, 4096);
    hal_flash_program(PROT_CONFIG_KEY_FLASH_ADDR, strlen(s_zj_appkey), s_zj_appkey);
#endif

    memset(test_buf, 0x0, sizeof(test_buf));
    hal_flash_read(PROT_CONFIG_KEY_FLASH_ADDR, sizeof(test_buf), test_buf);
    hexdump(0, "flash key", test_buf, sizeof(test_buf));

    easyflash_init();
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


void dump_task_stack_info(void)
{
    static UBaseType_t   s_arraySize = 0;
    UBaseType_t   task_num = 0;
    static TaskStatus_t  *s_task_status_array_p = NULL;
    uint32_t      FreeRTOSRunTimeTicks = 0;
    UBaseType_t   cnt = 0;

    static uint8_t is_runned = 0;

    if (is_runned == 0) {
        s_arraySize = uxTaskGetNumberOfTasks() + 5;
        s_task_status_array_p = pvPortMalloc(s_arraySize * sizeof(TaskStatus_t));
        LOG(LOG_LVL_ERROR, "alloc size:%d\r\n", s_arraySize * sizeof(TaskStatus_t));

        if (s_task_status_array_p != NULL) {
            is_runned = 1;
        } else {
            return;
        }
    }

    if (is_runned == 1) {
        task_num = uxTaskGetNumberOfTasks();
        if (task_num > s_arraySize) {
            task_num = s_arraySize;
        }

        cnt = uxTaskGetSystemState(
                        (TaskStatus_t *)s_task_status_array_p,
                        (UBaseType_t)task_num,
                        (uint32_t *)&FreeRTOSRunTimeTicks);

        LOG(LOG_LVL_INFO,"\r\n=============================================\r\n");
        LOG(LOG_LVL_INFO,
                "TaskName    Priority    TaskNumber    MinStk    tick\r\n");
        for(int x = 0; x < cnt; x++) {
            LOG(LOG_LVL_INFO,
                "%8.8s    %02d          %02d            %06d    %ld\r\n",
                s_task_status_array_p[x].pcTaskName,
                (int)s_task_status_array_p[x].uxCurrentPriority,
                (int)s_task_status_array_p[x].xTaskNumber,
                (int)s_task_status_array_p[x].usStackHighWaterMark * sizeof(StackType_t),
                (int)((float)s_task_status_array_p[x].ulRunTimeCounter/FreeRTOSRunTimeTicks*100));
        }
        LOG(LOG_LVL_INFO,"=============================================\r\n");
    }
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
            dump_task_stack_info();
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
    if(OS_OK != OS_ThreadCreate(&g_temp_thread, "TempAPP", temp_app_task_entry, NULL, 1, TEMP_APP_TASK_STACK_SIZE)) {
        LN_ASSERT(1);
    }
#endif
}
