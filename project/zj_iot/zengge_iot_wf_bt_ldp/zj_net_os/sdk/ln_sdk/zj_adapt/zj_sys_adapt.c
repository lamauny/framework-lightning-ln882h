#include "zj_adapt_api.h"
#include "zj_adapt_config.h"
#include "freertos_common.h"
#include "debug/log.h"
#include "hal/hal_flash.h"
#include "reboot_trace/reboot_trace.h"
#include "ln_kv_api.h"
#include "hal/hal_wdt.h"
#include "ln882h.h"
#include "ln_utils.h"
#include "easyflash.h"

#define    TAG    "SYSAPT"
#define    ZJ_LOCAL_LOG_LVL LOG_LVL_INFO

#undef ZJ_LOG
#define ZJ_LOG(fmt, ...)  LOG(ZJ_LOCAL_LOG_LVL, "["TAG"]"fmt, ##__VA_ARGS__)

static uint8_t ln_wdt_is_inited = 0;
static volatile uint8_t ln_wdt_is_enabled = 0;
static OS_Thread_t g_adapt_wdt_thr = {.handle = NULL};

uint32_t zj_ada_get_time(void)
{
    return OS_GetTicks();
}

/**
 * @brief get free heap
 * @note 
 * @param[in] none
 * @return free heap
 */
uint32_t zj_get_freeheap()
{
    return OS_GetFreeHeapSize();
}

void zj_flash_write(uint32_t addr ,uint8_t *pbuf,int len)
{
    hal_flash_program(addr, len, pbuf);
}

void zj_flash_read(uint32_t addr ,uint8_t *pbuf,int len)
{
    hal_flash_read_by_cache(addr, len, pbuf);
}

void zj_flash_erase(uint32_t addr,uint32_t size)
{
    hal_flash_erase(addr, size);
}

void zj_restore_userdata()
{
    if (ln_wdt_is_enabled) {
        hal_wdt_cnt_restart(WDT_BASE); /* feed dog */
    }
    hal_flash_erase(PORT_CONFIG_USER_DATA_FLASH_ADDR, PORT_CONFIG_USER_DATA_SIZE);
    if (ln_wdt_is_enabled) {
        hal_wdt_cnt_restart(WDT_BASE); /* feed dog */
    }
    zj_restart_system();
}

void zj_userdata_delete_key(char *key)
{
    ef_del_and_save_env((const char*)key);
}

/**
 * @brief 保存用户数据
 * @note  基于key值的保存模块
 * @param[in] *key:对应key值
 * @param[in] *dat:需要保存的数据
 * @param[in] len :长度
 * @return 0:key not found  > 0 : save length
 */
int zj_userdata_write(char *key, uint8_t *dat,int len)
{
    if (ef_set_env_blob((const char *)key, (const void *)dat, len) != EF_NO_ERR) {
        ZJ_LOG("kv set <%s> failed!\r\n", key);
        return 0;
    }
    zj_adapter_post_event(ADAPT_EVT_STORE,NULL,NULL,0);
    return len;
}

/**
 * @brief 读取用户数据
 * @note  基于key值的保存模块
 * @param[in] *key:对应key值
 * @param[in] *dat:读出的数据
 * @param[in]  len:长度
 * @return 0:key not found  > 0 : save length
 */
int zj_userdata_read(char *key, uint8_t *dat,int len)
{
    size_t v_len = 0;
    ef_get_env_blob((const char*)key, dat, len, &v_len);
    ZJ_LOG("kv read <%s>, len:%d!\r\n", key, v_len);
    return v_len;
}

void hal_reboot()
{
    ln_chip_reboot();
}

static void local_adapt_wdt_entry(void *arg)
{
    LN_UNUSED(arg);

    while(1) {
        if (ln_wdt_is_enabled == 0) { /* wdt is not enabled */
            OS_MsDelay(5000); /* The timeout of the wdt needs to be greater than 5s */
        } else {
            hal_wdt_cnt_restart(WDT_BASE); /* feed dog */
            OS_MsDelay(500);
        }
    }
}

void WDT_IRQHandler()
{
    // ZJ_LOG("=== wdt irq handler, will reboot ===\r\n");
}

/**
 * @brief 开启看门狗
 * @note  
 * @param[in] none
 * @return none
 */
void zj_watchdog_start()
{
    if (ln_wdt_is_inited == 0) {
        wdt_init_t_def wdt_init;
        memset(&wdt_init, 0, sizeof(wdt_init));
        wdt_init.wdt_rmod = WDT_RMOD_1;         //等于0的时候，计数器溢出时直接复位，等于1的时候，先产生中断，如果再次溢出，则产生复位。
        wdt_init.wdt_rpl = WDT_RPL_32_PCLK;     //设置复位延时的时间
        wdt_init.top = WDT_TOP_VALUE_10;         //设置看门狗计数器的值,当TOP=1时，对应计数器的值为0x1FF，而看门狗是用的时钟是一个单独的32k时钟，
                                                //所以此时的喂狗时间必须在 (1/32k) * 0x1FF 内。
        hal_wdt_init(WDT_BASE, &wdt_init);

        NVIC_SetPriority(WDT_IRQn,     4);
        NVIC_EnableIRQ(WDT_IRQn);

        // hal_wdt_en(WDT_BASE,HAL_ENABLE);
        // hal_wdt_cnt_restart(WDT_BASE); /* feed dog */
        // ln_wdt_is_inited = 1;
        // ln_wdt_is_enabled = 1;
    } else {
        // hal_wdt_en(WDT_BASE, HAL_ENABLE);
        // ln_wdt_is_enabled = 1;
    }

    if (g_adapt_wdt_thr.handle == NULL) {
        OS_ThreadCreate(&g_adapt_wdt_thr, "wdtT", local_adapt_wdt_entry, NULL, PORT_CONFIG_ZG_WATCHDOG_TASK_PRIO, 256);
    }
}

/**
 * @brief 停止看门狗
 * @note  
 * @param[in] time : 等待时间
 * @return none
 */
int zj_watchdog_stop(uint32_t time)
{
    if (ln_wdt_is_inited == 0) {
        return 0;
    }

    hal_wdt_en(WDT_BASE, HAL_DISABLE);
    // hal_wdt_deinit();
    ln_wdt_is_enabled = 0;

    return 0;
}

zj_reset_reason_t zj_get_reset_reason()
{
    zj_reset_reason_t reason = RESET_REASON_UNKNOWN;
    chip_reboot_cause_t rst = ln_chip_get_reboot_cause();
    switch (rst) {
        case CHIP_REBOOT_POWER_ON:
            reason = RESET_REASON_PWR_ON;
            break;
        case CHIP_REBOOT_SOFTWARE:
            reason = RESET_REASON_SOFTWARE;
            break;
        case CHIP_REBOOT_WATCHDOG:
            reason = RESET_REASON_WDT;
            break;
    }
    return reason;
}
