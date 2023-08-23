
#include "zj_adapt_api.h"
#include "zj_adapt_config.h"
#include <stdbool.h>
#include "sys/time.h"

#define    TAG    "SYSAPT"
#define    ZJ_LOCAL_LOG_LVL LOG_LVL_INFO

#undef ZJ_LOG
#define ZJ_LOG(fmt, ...)  LOG(ZJ_LOCAL_LOG_LVL, "["TAG"]"fmt, ##__VA_ARGS__)

/**
 * +: east tz
 * -: west tz
 * 8: UTC+8
 * -8: UTC-8
*/
#define ZJ_RTC_TZ (8)

uint32_t  mktimev(struct rtc_time *tm)
{
    struct tm time_new = {
        .tm_sec = tm->tm_sec,
        .tm_min = tm->tm_min,
        .tm_hour = tm->tm_hour,
        .tm_mday = tm->tm_mday,
        .tm_mon = tm->tm_mon,   /* - 1 */
        .tm_year = tm->tm_year, /* - 1900 */
        .tm_wday = tm->tm_wday  /* 7 --> 0 */
    };

    return (uint32_t)(mktime(&time_new));
}

static bool g_is_time_set = false;
void  zj_rtc_set_time(struct rtc_time *time_set)
{
    time_t init_time;
    struct timeval now = {0};
    if (!g_is_time_set) {
        extern void linux_copat_time_init(void);
        tz_set(ZJ_RTC_TZ);
        linux_copat_time_init();
        ZJ_LOG("init linux copat time!\r\n");
    }

    struct tm time_new = {
        .tm_sec = time_set->tm_sec,
        .tm_min = time_set->tm_min,
        .tm_hour = time_set->tm_hour,
        .tm_mday = time_set->tm_mday,
        .tm_mon = time_set->tm_mon,  /* - 1 */
        .tm_year = time_set->tm_year,/* - 1900 */
        .tm_wday = time_set->tm_wday /* 7 --> 0 */
    };

    init_time = mktime(&time_new);
    now.tv_sec = init_time;
    settimeofday(&now, NULL);
    g_is_time_set = true;
    ZJ_LOG("\r\nrtc init \r\n");
}

/**
 * struct rtc_time
 * 出参代表的是真实的时间，跟 struct tm 不一样
 * 出参的时间是加入了时区支持的，也就是获取到的就是当前时区的时间，
 * 如果需要时间准确，请务必配置正确的时区 tz_set
*/
void  zj_rtc_get_time(struct rtc_time *RTC_return)
{
    struct tm t;
    struct timeval ts_now;
    time_t timep;
    if (!RTC_return) {
        return;
    }

    time(&timep); /* UTC timestamp */
    localtime_r(&timep, &t); /* include tz */

    RTC_return->tm_sec = t.tm_sec;
    RTC_return->tm_min = t.tm_min;
    RTC_return->tm_hour = t.tm_hour;
    RTC_return->tm_mday = t.tm_mday;
    RTC_return->tm_mon = t.tm_mon + 1;
    RTC_return->tm_year = t.tm_year + 1900;
    RTC_return->tm_wday = t.tm_wday;
    if(RTC_return->tm_wday == 0)
    {
        RTC_return->tm_wday = 7;
    }

    ZJ_LOG("local time: y:%d-m:%d-d:%d; h:%d-m:%d-s:%d-w:%d\r\n",
        t.tm_year, t.tm_mon, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, t.tm_wday);
}

uint32_t zj_get_current_timestamp()
{
    time_t timep;
    time(&timep); /* UTC timestamp */
    return (uint32_t)timep;
}

uint8_t is_zj_rtc_time_set()
{
    return g_is_time_set;
}
