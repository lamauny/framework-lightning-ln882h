#include "zj_adapt_api.h"
#include "zj_adapt_config.h"

// #include "esp_app_format.h"
// #include "esp_http_client.h"
// #include "esp_flash_partitions.h"
// #include "esp_partition.h"
#include "errno.h"
// #include "esp_ota_ops.h"
// #include "esp_https_ota.h"

static const char *TAG = "ESPOTA";
static int g_ota_status;
static uint32_t g_ota_take_time;
// extern const uint8_t server_cert_pem_start[] asm("_binary_ca_cert_pem_start");
// extern const uint8_t server_cert_pem_end[] asm("_binary_ca_cert_pem_end");
#define OTA_URL_SIZE                        256

#ifndef CONFIG_COMPRESS_OTA
// esp_err_t _http_event_handler(esp_http_client_event_t *evt);
#endif

// static esp_http_client_config_t config = {
//       //  .url = "http://wifij01cn.magichue.net:80/app/ota/download?version=35_02_20230303_ZG-ESP&old=35_63_20190907&type=35&filename=esp_ota.bin&checkcode={D3F189AEFF}&mac=68B6B32B777C",

// #ifdef CONFIG_EXAMPLE_USE_CERT_BUNDLE

//         .crt_bundle_attach = esp_crt_bundle_attach,
// #else

//         .cert_pem = (char *)server_cert_pem_start,
// #endif /* CONFIG_EXAMPLE_USE_CERT_BUNDLE */
// #ifndef CONFIG_COMPRESS_OTA
//         .event_handler = _http_event_handler,
// #endif
//         .keep_alive_enable = true,
// #ifdef CONFIG_EXAMPLE_FIRMWARE_UPGRADE_BIND_IF

//         .if_name = &ifr,
// #endif

//     };

void ota_data_dec(uint8_t key,uint8_t *inoutdata,int len)
{
    
}

#ifdef CONFIG_COMPRESS_OTA
#include "esp_crc.h"
#include "md/esp_md.h"
#include "../../../../zj_esp32c3_project/bootloader_components/include/bootloader_custom_ota.h"

#define BUFFSIZE 1024

/*an ota data write buffer ready to write to the flash*/

static char ota_write_data[BUFFSIZE + 1] = {0};

#define UPDATE_PARTITION_RESERVED_SPACE     (1024 * 8) // Reserve some space for signature information when secure boot used
#define BUFFER_SIZE 					    (1024 * 4)

static void print_md5_str(unsigned char digest[16]);

static void http_cleanup(esp_http_client_handle_t client)
{
    esp_http_client_close(client);
    esp_http_client_cleanup(client);
}

static esp_err_t example_storage_compressed_image(const esp_partition_t *update_partition)
{

#ifdef CONFIG_EXAMPLE_SKIP_COMMON_NAME_CHECK
    config.skip_cert_common_name_check = true;
#endif

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL) {
        ESP_LOGE(TAG, "Failed to initialise HTTP connection");
        g_ota_status = 2;
        zj_adapter_post_event(ADAPT_EVT_OTA_STATUS,&g_ota_status,NULL,0);
        return ESP_FAIL;
    }
    esp_err_t err = esp_http_client_open(client, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        g_ota_status = 2;
        zj_adapter_post_event(ADAPT_EVT_OTA_STATUS,&g_ota_status,NULL,0);
        return ESP_FAIL;
    }

    int total_content_length = esp_http_client_fetch_headers(client);
    if (total_content_length < 0) {
        ESP_LOGE(TAG, "HTTP client fetch headers failed");
    } else {
        ESP_LOGW(TAG, "%d bytes to be downloaded", total_content_length);
        g_ota_status = 0;
        zj_adapter_post_event(ADAPT_EVT_OTA_STATUS,&g_ota_status,NULL,0);
    }
    
    bootloader_custom_ota_header_t custom_ota_header = {0};
    bool image_header_was_checked = false;
    uint32_t wrote_size = 0;
    uint8_t progress_comp = 0;
    /*write compressed ota image to the partiton*/
    while (1) {
        int data_read = esp_http_client_read(client, ota_write_data, BUFFSIZE);
        if (data_read < 0) {
            ESP_LOGE(TAG, "Error: SSL data read error");
            g_ota_status = 2;
            goto err;
        } else if (data_read > 0) {
            if (image_header_was_checked == false) {
                if (data_read > sizeof(bootloader_custom_ota_header_t)) {
                    memcpy(&custom_ota_header, ota_write_data, sizeof(bootloader_custom_ota_header_t));
                    // check whether it is a compressed app.bin(include diff compressed ota and compressed only ota).
                    if (!strcmp((char *)custom_ota_header.header.magic, BOOTLOADER_CUSTOM_OTA_HEADER_MAGIC)) {
                        ESP_LOGW(TAG, "compressed ota detected");
                    }

                    // Check compressed image bin size
                    if (custom_ota_header.header.length > update_partition->size - UPDATE_PARTITION_RESERVED_SPACE) {
                        ESP_LOGE(TAG, "Compressed image size is too large");
                        g_ota_status = -1;
                        goto err;
                    }
                    image_header_was_checked = true;
                } else {
                    ESP_LOGE(TAG, "received package is not fit len");
                    g_ota_status = 7;
                    goto err;
                }
            }
            int progress = wrote_size * 100 / total_content_length;
            ESP_LOGI(TAG, "download:%ld tol:%d (%%%d) (%ldms)",
                    wrote_size,
                    total_content_length,
                    progress,
                    (uint32_t)(PORT_CONFIG_SYSTEM_TIME - g_ota_take_time)
                    );
            if(progress_comp != progress && progress % 5 == 0){

                progress_comp = progress;
                g_ota_status = 0;
                zj_adapter_post_event(ADAPT_EVT_OTA_STATUS,&g_ota_status,NULL,(int)progress_comp);
            }
            err = esp_partition_write(update_partition, wrote_size, ota_write_data, data_read);
            if (err != ESP_OK) {
                ESP_LOGE(TAG, "write ota data error is %s", esp_err_to_name(err));
                g_ota_status = -1;
                goto err;
            }
            wrote_size += data_read;
        } else if (data_read == 0) {
            /*
             * As esp_http_client_read never returns negative error code, we rely on
             * `errno` to check for underlying transport connectivity closure if any
             */
            if (errno == ECONNRESET || errno == ENOTCONN) {
                ESP_LOGE(TAG, "Connection closed, errno = %d", errno);
                break;
            }
            if (esp_http_client_is_complete_data_received(client) == true) {
                ESP_LOGI(TAG, "Connection closed");
                break;
            }
        }
    }
    ESP_LOGI(TAG, "Total Write binary data length: %ld", wrote_size);
    if (esp_http_client_is_complete_data_received(client) != true) {
        ESP_LOGE(TAG, "Error in receiving complete file");
        g_ota_status = 2;
        goto err;
    }
    http_cleanup(client);
    return ESP_OK;
err:
    zj_adapter_post_event(ADAPT_EVT_OTA_STATUS,&g_ota_status,NULL,progress_comp);
    http_cleanup(client);
    return ESP_FAIL;
}


static esp_err_t get_custom_ota_head(const esp_partition_t *partition, bootloader_custom_ota_header_t *custom_ota_header)
{
    return esp_partition_read(partition, 0, custom_ota_header, sizeof(bootloader_custom_ota_header_t));
}

// Get the custom ota header from partition where compressed firmware is stored
static bool process_custom_ota_head(const esp_partition_t *partition)
{
    bootloader_custom_ota_header_t c_header = {0};
    uint32_t bootloader_custom_ota_header_length;
    uint32_t crc_num = 0;
    esp_err_t err = ESP_FAIL;

    err = get_custom_ota_head(partition, &c_header);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "compressed image get fail, err: %0xd", err);
        return false;
    }

    // check magic num
    if (strcmp((char *)c_header.header.magic, BOOTLOADER_CUSTOM_OTA_HEADER_MAGIC)) {
        ESP_LOGE(TAG, "magic num err");
        return false;
    }

    if (c_header.header.version > BOOTLOADER_CUSTOM_OTA_HEADER_VERSION) {
        ESP_LOGE(TAG, "compress version err");
        return false;
    }
    ESP_LOGI(TAG, "compress type is %u, delta type is %u.", c_header.header.compress_type, c_header.header.delta_type);
    ESP_LOGI(TAG, "encryption type is %u, header version is %u.", c_header.header.encryption_type, c_header.header.version);

    if (c_header.header.version == 1) {
        bootloader_custom_ota_header_length = sizeof(bootloader_custom_ota_header_v1_t);
        crc_num = c_header.header_v1.crc32;
    } else {
        bootloader_custom_ota_header_length = sizeof(bootloader_custom_ota_header_v2_t);
        crc_num = c_header.header_v2.crc32;
    }

    // check crc checksum
    if (esp_crc32_le(0, (const uint8_t *)&c_header, bootloader_custom_ota_header_length - sizeof(c_header.header_v2.crc32)) != crc_num) {
        ESP_LOGE(TAG, "crc verify failed");
        return false;
    }

    // check md5 checksum
    // Read comresssed data and compute the digest chunk by chunk
    uint8_t md5_digest[16];
    mbedtls_md5_context ctx;
    uint8_t *data = (uint8_t *)malloc(BUFFER_SIZE);
    if (data == NULL) {
        ESP_LOGE(TAG, "malloc fail");
    }
    uint32_t len = 0;
    uint32_t total_read_len = 0;
    esp_md5_init(&ctx);
    do {
        len = (c_header.header.length - total_read_len > BUFFER_SIZE) ? BUFFER_SIZE : (c_header.header.length - total_read_len);
        err = esp_partition_read(partition, bootloader_custom_ota_header_length + total_read_len, data, len);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "partition read errno is %0xd", err);
        }
        esp_md5_update(&ctx, (unsigned const char *)data, len);
        total_read_len += len;
    } while (total_read_len < c_header.header.length);

    esp_md5_finish(&ctx, md5_digest);

    if (memcmp(c_header.header.md5, md5_digest, sizeof(md5_digest))) {
        ESP_LOGE(TAG, "md5 verify failed");
        return false;
    }
    print_md5_str(md5_digest);
    esp_md5_free(&ctx);

    if (c_header.header.delta_type) {
        const esp_partition_t *running = esp_ota_get_running_partition();
        len = 0;
        crc_num = 0;

        for (uint32_t loop = 0; loop < c_header.header_v2.base_len_for_crc32;) {
            len = (c_header.header_v2.base_len_for_crc32 - loop > BUFFER_SIZE) ? BUFFER_SIZE : (c_header.header_v2.base_len_for_crc32 - loop);
            err = esp_partition_read(running, loop, data, len);
            if (err != ESP_OK) {
                ESP_LOGE(TAG, "partition read errno is %0xd", err);
            }
            crc_num = esp_crc32_le(crc_num, (const uint8_t *)data, len);
            loop += len;
        }

        // check crc checksum
        if (crc_num != c_header.header_v2.base_crc32) {
            ESP_LOGE(TAG, "base crc verify failed");
            return false;
        }
    }

    free(data);
    data = NULL;
    return true;
}


#if CONFIG_EXAMPLE_CLEAR_STORAGE_IF_CHECK_FAILED
// Verify the stored compressed data. If the verification fails, call this function to clear some data of the compressed partition
static esp_err_t custom_ota_clear_storage_header(const esp_partition_t *storage_partition)
{
#define SEC_SIZE    (1024 * 4)
    return esp_partition_erase_range(storage_partition, 0, SEC_SIZE);
}
#endif

static int __ota_task()
{
    esp_err_t err;
    const esp_partition_t *update_partition = NULL; // This update partition is used to store compressed ota image data.

    ESP_LOGI(TAG, "Starting OTA example");

    const esp_partition_t *configured = esp_ota_get_boot_partition();
    const esp_partition_t *running = esp_ota_get_running_partition();

    if (configured != running) {
        ESP_LOGW(TAG, "Configured OTA boot partition at offset 0x%08lx, but running from offset 0x%08lx",
            configured->address, running->address);
        ESP_LOGW(TAG, "(This can happen if either the OTA boot data or preferred boot image become corrupted somehow.)");
    }
    ESP_LOGI(TAG, "Running partition type %d subtype %d (offset 0x%08lx)",
        running->type, running->subtype, running->address);


    // In this example, we always store compressed app.bin to ota_1 partition.
    update_partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, BOOTLOADER_CUSTOM_OTA_PARTITION_SUBTYPE, NULL);
    assert(update_partition != NULL);
    ESP_LOGI(TAG, "Writing to partition subtype %d at offset 0x%lx",
        update_partition->subtype, update_partition->address);

    err = esp_partition_erase_range(update_partition, 0, update_partition->size);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to erase partition: %s", esp_err_to_name(err));
        return -1;
    }

    if (example_storage_compressed_image(update_partition) != ESP_OK) {
        ESP_LOGE(TAG, "Storage failed");
        return -1;
    }

    // check downloaded compressed image 
    if (process_custom_ota_head(update_partition) != true) {
        ESP_LOGE(TAG, "verify compressed image header failed");
#if CONFIG_EXAMPLE_CLEAR_STORAGE_IF_CHECK_FAILED
        custom_ota_clear_storage_header(update_partition);
#endif
        return -1;
    }

    ESP_LOGI(TAG, "running->subtype: %d, update->subtype: %d.", running->subtype, update_partition->subtype);
    ESP_LOGI(TAG, "Prepare to restart system!");
    return 0;
}


static void print_md5_str(unsigned char digest[16])
{
    // Create a string of the digest
    char digest_str[16 * 2];

    for (int i = 0; i < 16; i++) {
        sprintf(&digest_str[i * 2], "%02x", (unsigned int)digest[i]);
    }

    // For reference, MD5 should be deeb71f585cbb3ae5f7976d5127faf2a
    ESP_LOGI(TAG, "Computed MD5 hash of the upload: %s", digest_str);
}

#else

// esp_err_t _http_event_handler(esp_http_client_event_t *evt)
// {
//     switch (evt->event_id) {
//     case HTTP_EVENT_ERROR:
//         ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
//         g_ota_status = 2;
//         zj_adapter_post_event(ADAPT_EVT_OTA_STATUS,&g_ota_status,"http timeout",0);
//         break;
//     case HTTP_EVENT_ON_CONNECTED:{
//         ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
//         g_ota_status = 0;
//         zj_adapter_post_event(ADAPT_EVT_OTA_STATUS,&g_ota_status,NULL,0);
//     }break;
//     case HTTP_EVENT_HEADER_SENT:
//         ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
//         break;
//     case HTTP_EVENT_ON_HEADER:
//         ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
//         break;
//     case HTTP_EVENT_ON_DATA:
//         ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
//         break;
//     case HTTP_EVENT_ON_FINISH:
//         ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
//         break;
//     case HTTP_EVENT_DISCONNECTED:
//         ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
//         break;
//     case HTTP_EVENT_REDIRECT:
//         ESP_LOGD(TAG, "HTTP_EVENT_REDIRECT");
//         break;
//     }
//     return ESP_OK;
// }


// esp_err_t zj_https_ota(const esp_https_ota_config_t *ota_config)
// {
//     if (ota_config == NULL || ota_config->http_config == NULL) {
//         ESP_LOGE(TAG, "esp_https_ota: Invalid argument");
//         return ESP_ERR_INVALID_ARG;
//     }

//     esp_https_ota_handle_t https_ota_handle = NULL;
//     esp_err_t err = esp_https_ota_begin(ota_config, &https_ota_handle);
//     if (https_ota_handle == NULL) {
//         return ESP_FAIL;
//     }
//     int content = esp_https_ota_get_image_size(https_ota_handle);
//     if(content == -1){

//         g_ota_status = 2;
//         zj_adapter_post_event(ADAPT_EVT_OTA_STATUS,&g_ota_status,"http timeout",0);
//     }
//     uint8_t progress_comp = 0,progress;
//     while (1) {
//         err = esp_https_ota_perform(https_ota_handle);
//         if (err != ESP_ERR_HTTPS_OTA_IN_PROGRESS) {
//             break;
//         }
//         int download = esp_https_ota_get_image_len_read(https_ota_handle);
//         progress = download * 100 / content;
//         ESP_LOGI(TAG, "download:%d tol:%d (%%%d) (%ldms)",
//         download,
//         content,
//         progress,
//         (uint32_t)(PORT_CONFIG_SYSTEM_TIME - g_ota_take_time)
//         );
//         if(progress_comp != progress && progress % 5 == 0){

//            progress_comp = progress;
//            zj_adapter_post_event(ADAPT_EVT_OTA_STATUS,&g_ota_status,NULL,progress);
//         }
//     }
//     if (err != ESP_OK) {
//         esp_https_ota_abort(https_ota_handle);
//         return err;
//     }

//     esp_err_t ota_finish_err = esp_https_ota_finish(https_ota_handle);
//     if (ota_finish_err != ESP_OK) {
//         return ota_finish_err;
//     }
//     return ESP_OK;
// }


int __ota_task()
{
// #ifdef CONFIG_EXAMPLE_FIRMWARE_UPGRADE_BIND_IF

//     esp_netif_t *netif = get_example_netif_from_desc(bind_interface_name);
//     if (netif == NULL) {
//         ESP_LOGE(TAG, "Can't find netif from interface description");
//         abort();
//     }
//     struct ifreq ifr;
//     esp_netif_get_netif_impl_name(netif, ifr.ifr_name);
//     ESP_LOGI(TAG, "Bind interface name is %s", ifr.ifr_name);
// #endif


// #ifdef CONFIG_EXAMPLE_FIRMWARE_UPGRADE_URL_FROM_STDIN

//     char url_buf[OTA_URL_SIZE];
//     if (strcmp(config.url, "FROM_STDIN") == 0) {
//         example_configure_stdin_stdout();
//         fgets(url_buf, OTA_URL_SIZE, stdin);
//         int len = strlen(url_buf);
//         url_buf[len - 1] = '\0';
//         config.url = url_buf;
//     } else {
//         ESP_LOGE(TAG, "Configuration mismatch: wrong firmware upgrade image url");
//         abort();
//     }
// #endif


// #ifdef CONFIG_EXAMPLE_SKIP_COMMON_NAME_CHECK

//     config.skip_cert_common_name_check = true;
// #endif


// esp_https_ota_config_t ota_config = {
//         .http_config = &config,
//     };
//     ESP_LOGI(TAG, "Attempting to download update from %s", config.url);
//     esp_err_t ret = zj_https_ota(&ota_config);
//     if (ret == ESP_OK) {
//         ESP_LOGI(TAG, "OTA Succeed, Rebooting...");
//         return 0;
//     } else {
//         ESP_LOGE(TAG, "Firmware upgrade failed");
//         return -1;
//     }
    return 0;
}


#endif
/** 
 * @brief 开启在线更新
 * @note  
 * @param[in] http_url:url
 * @return 0:success -1:fail
 */
int zj_ota_get_image(char *http_url)
{
//   g_ota_take_time = PORT_CONFIG_SYSTEM_TIME;
//   config.url = http_url;
//   zj_ble_scan_stop();
  return __ota_task();
}
/** 
 * @brief ota固件校验
 * @note  
 * @param[in] none
 * @return 0:success -1:fail
 */
int zj_ota_verify_image()
{
  return 0;
}
/** 
 * @brief reboot
 * @note  
 * @param[in] none
 * @return none
 */
void zj_ota_reboot()
{
    // esp_restart();
}