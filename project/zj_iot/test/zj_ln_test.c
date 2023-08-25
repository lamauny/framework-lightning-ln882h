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

#include "ZG_sys_config.h"
#include "zj_adapt_config.h"
#include "ZG_system.h"
#include "sys/time.h"
#include "zj_adapt_api.h"

#define ZJ_LOG_TEST
#define ZJ_RTC_TEST
#define LN_LINUX_COMPAT_TEST
#define ZJ_WIFI_TEST
#define ZJ_GPIO_TEST

#ifdef ZJ_LOG_TEST
static void zj_log_test(void)
{
    LOG(LOG_LVL_INFO, "zj log test\r\n");

    /* zj log test */
    {
        OS_LOGD("os-log-test", "debug:%d\n", __LINE__);
        OS_LOGW("os-log-test", "warning:%d\n", __LINE__);
        OS_LOGI("os-log-test", "info:%d\r\n", __LINE__);
        OS_LOGE("os-log-test", "error:%d\r\n", __LINE__);
        APP_LOGD("app-log-test", "debug:%d", __LINE__);
        APP_LOGW("app-log-test", "warning:%d", __LINE__);
        APP_LOGI("app-log-test", "info:%d", __LINE__);
        APP_LOGE("app-log-test", "error:%d", __LINE__);
        MESH_LOGD("mesh-log-test", "debug:%d", __LINE__);
        MESH_LOGW("mesh-log-test", "warning:%d", __LINE__);
        MESH_LOGI("mesh-log-test", "info:%d", __LINE__);
        MESH_LOGE("mesh-log-test", "error:%d", __LINE__);

        zj_dbg_printf("zj dbg printf test");
        zj_dbg_printf("zj dbg printf test:%d\r\n", __LINE__);
        zj_dbg_printf("zj dbg printf test\n");
        zj_printf("zj printf test");
        zj_printf("zj printf test:%d\r\n", __LINE__);
        zj_printf("zj printf test\n");

        os_printf("os printf test");
        os_printf("os printf test:%d\r\n", __LINE__);
        os_printf("os printf test\n");
    }
    LOG(LOG_LVL_INFO, "=== zj log test end ===\r\n");
}
#endif

#ifdef LN_LINUX_COMPAT_TEST
#define TEST_TIME_ZONE (-8)
static void ln_time_test(void)
{
    time_t t;
    struct tm *p = NULL;
    struct tm tm;
    struct timeval tv;
    extern void linux_copat_time_init(void);

    LOG(LOG_LVL_INFO, "==== UTC+%d time test ====\r\n", TEST_TIME_ZONE);
    tz_set(TEST_TIME_ZONE);
    linux_copat_time_init();

    // SET
    memset(&tv, 0, sizeof(tv));
    tv.tv_sec = 1703209259;
    settimeofday(&tv, NULL);

    time(&t); /* UTC timestamp */
    LOG(LOG_LVL_INFO, "local time now:%s; timestamp: %ld\r\n", ctime(&t), t);

    p = gmtime(&t);
    LOG(LOG_LVL_INFO, "gmtime:%d-%d-%d %d:%d:%d-w:%d\r\n",
        p->tm_year + 1900, p->tm_mon + 1, p->tm_mday,
        p->tm_hour, p->tm_min, p->tm_sec, p->tm_wday);

    memset(&tv, 0, sizeof(tv));
    gettimeofday(&tv, NULL);
    LOG(LOG_LVL_INFO, "gettimeofday secs:%ld\r\n", tv.tv_sec);

    localtime_r(&t, &tm);
    p = &tm;
    LOG(LOG_LVL_INFO, "localtime: %d-%d-%d %d:%d:%d-w:%d\r\n",
        p->tm_year + 1900, p->tm_mon + 1, p->tm_mday,
        p->tm_hour, p->tm_min, p->tm_sec, p->tm_wday);

    t = mktime(p);
    LOG(LOG_LVL_INFO, "mktime :%s; timestamp: %ld\r\n", ctime(&t), t);

    LOG(LOG_LVL_INFO, "==== UTC+%d time test end ====\r\n", TEST_TIME_ZONE);

    for (int i = 0; i < 10; i++)
    {
        time(&t); /* UTC timestamp */
        LOG(LOG_LVL_INFO, "local time now:%s; %ld\r\n", ctime(&t), t);
        OS_MsDelay(1000);
    }
}
#endif

#ifdef ZJ_RTC_TEST
static void zj_rtc_test(void)
{
    /* RTC TEST */
    extern void zj_rtc_set_time(struct rtc_time *time_set);
    extern void zj_rtc_get_time(struct rtc_time *RTC_return);
    extern uint32_t zj_get_current_timestamp();
    extern uint32_t mktimev(struct rtc_time *tm);
    extern uint8_t is_zj_rtc_time_set();
    {
        struct rtc_time test;
        uint32_t timestamp = 0;
        uint32_t timestamp1 = 0;
        struct rtc_time now = {
            .tm_year = 2023 - 1900,
            .tm_mon  = 8 - 1,
            .tm_mday = 22,
            .tm_hour = 16,
            .tm_min  = 6,
            .tm_sec  = 30,
            // .tm_wday = 2,
        };
        LOG(LOG_LVL_INFO, "=== rtc test ===\r\n");
        tz_set(8);

        if (is_zj_rtc_time_set() == 0) {
            zj_rtc_set_time(&now);
        }

        for (int i = 0; i < 10; i++)
        {
            zj_rtc_get_time(&test);
            timestamp = zj_get_current_timestamp();
            timestamp1 = mktimev(&test);

            LOG(LOG_LVL_INFO, "struct time: %d-%d-%d; %d:%d:%d-w:%d\r\n",
                test.tm_year + 1900, test.tm_mon + 1, test.tm_mday,
                test.tm_hour, test.tm_min, test.tm_sec, test.tm_wday);
            LOG(LOG_LVL_INFO, "timestamp:%ld; %ld\r\n", timestamp, timestamp1);
            OS_MsDelay(1000);
        }
    }
}
#endif

#ifdef ZJ_GPIO_TEST
#include "zj_adapt_api.h"
#include "zj_adapt_config.h"

void zj_gpio_test(void)
{
    zj_gpio_init(PORT_CONFIG_PLUG_LED_RED, ZJ_GPIO_OUTPUT);
    zj_gpio_output_set(PORT_CONFIG_PLUG_LED_RED, 0);
    OS_MsDelay(100);
    zj_gpio_output_set(PORT_CONFIG_PLUG_LED_RED, 1);
    OS_MsDelay(100);
    zj_gpio_output_set(PORT_CONFIG_PLUG_LED_RED, 0);
}
#endif

void zj_ln_adapt_test(void)
{
#ifdef ZJ_LOG_TEST
    zj_log_test();
#endif

#ifdef LN_LINUX_COMPAT_TEST
    ln_time_test();
#endif

#ifdef ZJ_RTC_TEST
    zj_rtc_test();
#endif

#ifdef ZJ_GPIO_TEST
    zj_gpio_test();
#endif

#ifdef ZJ_WIFI_TEST
    #define TEST_SSID "MurphyTest"
    #define TEST_PWD "12345678"
    extern void zj_wifi_drv_init();
    OS_MsDelay(1000);
    extern void zj_wifi_STA_Start(uint8_t *ssid,uint8_t ssid_len,uint8_t *pwd,uint8_t pwd_len);
    zj_wifi_STA_Start(TEST_SSID, strlen(TEST_SSID), TEST_PWD, strlen(TEST_PWD));
    OS_MsDelay(8000);
    while(1) {
        zj_scan_router(ADAPT_EVT_BLUFI_SCAN);
        OS_MsDelay(10000);
    }
#endif
}
