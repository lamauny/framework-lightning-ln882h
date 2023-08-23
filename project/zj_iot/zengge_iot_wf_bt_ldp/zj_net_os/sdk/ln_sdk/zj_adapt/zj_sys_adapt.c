#include "zj_adapt_api.h"
#include "zj_adapt_config.h"
#include "freertos_common.h"
#include "debug/log.h"
#include "hal/hal_flash.h"
#include "reboot_trace/reboot_trace.h"
#include "ln_kv_api.h"
#include "hal/hal_wdt.h"
#include "ln882h.h"

#define    TAG    "SYSAPT"
#define    ZJ_LOCAL_LOG_LVL LOG_LVL_INFO

#undef ZJ_LOG
#define ZJ_LOG(fmt, ...)  LOG(ZJ_LOCAL_LOG_LVL, "["TAG"]"fmt, ##__VA_ARGS__)

#define STORAGE_NAMESPACE "storage"

uint32_t esp_timer_get_time(void)
{
    OS_GetTime();
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
    // nvs_handle_t my_handle;
    // esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    // if (err != ESP_OK) return;
    // nvs_erase_all(my_handle);
    ZJ_LOG("[%s:%d] Murphy TODOs\r\n", __func__, __LINE__);
}

void hal_reboot()
{
    ln_chip_reboot();
}

void zj_userdata_delete_key(char *key)
{
    ln_kv_del((const char*)key);
    // zj_adapter_post_event(ADAPT_EVT_STORE,NULL,NULL,0);
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
    if (ln_kv_set((const char *)key, (const char *)dat, len) != KV_ERR_NONE) {
        ZJ_LOG("kv set <%s> failed!\r\n", key);
        return 0;
    }
    // zj_adapter_post_event(ADAPT_EVT_STORE,NULL,NULL,0);
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
    if (ln_kv_get((const char*)key, dat, len, &v_len) != KV_ERR_NONE) {
        ZJ_LOG("kv get <%s> failed!\r\n", key);
        return 0;
    }
    return v_len;
}


static uint8_t ln_wdt_is_inited = 0;

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
        hal_wdt_en(WDT_BASE,HAL_ENABLE);
        ln_wdt_is_inited = 1;
    } else {
        hal_wdt_en(WDT_BASE, HAL_ENABLE);
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
    hal_wdt_deinit();

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
