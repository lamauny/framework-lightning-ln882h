#include "zj_adapt_config.h"
// #include "esp_app_format.h"
// #include "esp_flash_partitions.h"
// #include "esp_ota_ops.h"
#include "debug/log.h"

#define HASH_LEN 32 /* SHA-256 digest length */

extern int aes_key_init();
extern void zj_adapter_init();;
extern void sys_dbg_init(); 
extern void net_type_init();
extern void zj_watchdog_start();

void ota_boot_init()
{
//     uint8_t sha_256[HASH_LEN] = { 0 };
//     esp_partition_t partition;

//     // get sha256 digest for the partition table
//     partition.address   = ESP_PARTITION_TABLE_OFFSET;
//     partition.size      = ESP_PARTITION_TABLE_MAX_LEN;
//     partition.type      = ESP_PARTITION_TYPE_DATA;
//     esp_partition_get_sha256(&partition, sha_256);
//     print_sha256(sha_256, "SHA-256 for the partition table: ");

//     // get sha256 digest for bootloader
//     partition.address   = ESP_BOOTLOADER_OFFSET;
//     partition.size      = ESP_PARTITION_TABLE_OFFSET;
//     partition.type      = ESP_PARTITION_TYPE_APP;
//     esp_partition_get_sha256(&partition, sha_256);
//     print_sha256(sha_256, "SHA-256 for bootloader: ");

//     // get sha256 digest for running partition
//     esp_partition_get_sha256(esp_ota_get_running_partition(), sha_256);
//     print_sha256(sha_256, "SHA-256 for current firmware: ");
// #if 0
//     const esp_partition_t *running = esp_ota_get_running_partition();
//     esp_ota_img_states_t ota_state;
//     if (esp_ota_get_state_partition(running, &ota_state) == ESP_OK) {

//         if (ota_state == ESP_OTA_IMG_PENDING_VERIFY) {
          
//            ESP_LOGI("sha", "Diagnostics completed successfully! Continuing execution ...");
//            esp_ota_mark_app_valid_cancel_rollback();
//         }
//         ESP_LOGI("ota state", "%d",ota_state);
//     }
//     #endif
}

// extern esp_netif_t *sta_netif;
// extern esp_netif_t *ap_netif;
void zengge_common_main_init()
{
    // ESP_ERROR_CHECK(esp_netif_init());
    // ESP_ERROR_CHECK(esp_event_loop_create_default());
    // sta_netif = esp_netif_create_default_wifi_sta();
    // ap_netif = esp_netif_create_default_wifi_ap();
    // wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    // ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // aes_key_init();
    // zj_adapter_init();/* 适配器初始化初始化 */
    // net_type_init();  /* 网络类型初始化 */
    // #ifdef CONFIG_WIFI_BEST_PERFORMANCE_OPTION
    // extern uint8_t net_type_get_config();
    // if(net_type_get_config() != 0){

    //     esp_wifi_set_ps(WIFI_PS_NONE);
    //     ESP_LOGW("ps","WIFI_PS_NONE");
    // }
    // #endif
    zj_watchdog_start();
}
