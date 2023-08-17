#include "zj_adapt_api.h"
#include "zj_adapt_config.h"
#include "freertos_common.h"
#include "debug/log.h"

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
    // const esp_partition_t *partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, 0x11, "custom");
    // assert(partition != NULL);
    // // Write the data, starting from the beginning of the partition
    // esp_err_t err = esp_partition_write(partition, addr, pbuf, (size_t)len);
    // if (err != ESP_OK) {
    //     ESP_LOGE("flash write","err %x",err);
    // }
}

void zj_flash_read(uint32_t addr ,uint8_t *pbuf,int len)
{
    // const esp_partition_t *partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, 0x11, "custom");
    // assert(partition != NULL);
    // // Write the data, starting from the beginning of the partition
    // esp_err_t err = esp_partition_read(partition, addr, pbuf, (size_t)len);   
    // if (err != ESP_OK) {
    //     ESP_LOGE("flash read","err %x",err);
    // }
}

void zj_flash_erase(uint32_t addr,uint32_t size)
{
  // const esp_partition_t *partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, 0x11, "custom");
  // assert(partition != NULL);
  // esp_err_t err = esp_partition_erase_range(partition, addr, (size_t)size);
  // if (err != ESP_OK) {

  //     ESP_LOGE("flash erase","err %x",err);
  // }
}

void zj_restore_userdata()
{
  // nvs_handle_t my_handle;
  // esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
  // if (err != ESP_OK) return;
  // nvs_erase_all(my_handle);
}

void hal_reboot()
{
  //  esp_restart();
}

void zj_userdata_delete_key(char *key)
{
  // nvs_handle_t my_handle;
  // esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
  // if (err != ESP_OK) return ;

  // nvs_erase_key(my_handle,key);
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
  // nvs_handle_t my_handle;
  // esp_err_t err;

  // // Open
  // err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
  // if (err != ESP_OK) return err;
  // err = nvs_set_blob(my_handle, key, dat, (size_t)len);

  // if (err != ESP_OK) return err;

  // err = nvs_commit(my_handle);
  // if (err != ESP_OK) return err;

  // nvs_close(my_handle);
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
  // nvs_handle_t my_handle;
  // esp_err_t err;

  // // Open
  // err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
  // if (err != ESP_OK) return 0;
  // err = nvs_get_blob(my_handle, key, dat, (size_t *)(&len));

  // if (err != ESP_OK) return 0;

  // err = nvs_commit(my_handle);
  // if (err != ESP_OK) return 0;

  // nvs_close(my_handle);                                   
  return len;
}
/**
 * @brief 开启看门狗
 * @note  
 * @param[in] none
 * @return none
 */
void zj_watchdog_start()
{

}
/**
 * @brief 停止看门狗
 * @note  
 * @param[in] time : 等待时间
 * @return none
 */
int zj_watchdog_stop(uint32_t time)
{

  return 0;
}

zj_reset_reason_t zj_get_reset_reason()
{
    // esp_reset_reason_t rstinfo = esp_reset_reason(void);();
    // os_printf("\n\n[esp reset reason %d]\n\n",rstinfo);
    // switch (rstinfo) {
      
    //   case  ESP_RST_POWERON :
    //   return RESET_REASON_PWR_ON;
    //   case  ESP_RST_EXT :       //!< Reset by external pin (not applicable for ESP32)
    //   return RESET_REASON_HARFWARE;
    //   case  ESP_RST_SW :        //!< Software reset via esp_restart
    //   return RESET_REASON_SOFTWARE;
    //   case  ESP_RST_PANIC :      //!< Software reset due to exception/panic
    //   return RESET_REASON_CRASH;
    //   case  ESP_RST_INT_WDT :    //!< Reset (software or hardware) due to interrupt watchdog
    //   case  ESP_RST_TASK_WDT :   //!< Reset due to task watchdog
    //   case  ESP_RST_WDT :      //!< Reset due to other watchdogs
    //   return RESET_REASON_WDT;
    //   break;
    //   case ESP_RST_BROWNOUT,   //!< Brownout reset (software or hardware)
    //   return RESET_REASON_BROWN_OUT;
    //   default:
    //   return RESET_REASON_UNKNOWN;
    // }
    return RESET_REASON_UNKNOWN;
}