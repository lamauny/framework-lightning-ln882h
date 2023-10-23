#include "zj_adapt_api.h"
#include "zj_adapt_config.h"

#include "flash_partition_table.h"
#include "hal/hal_flash.h"
#include "netif/ethernetif.h"

#include "ota_port.h"
#include "ota_image.h"
#include "ota_types.h"
// #include "ln_kv_api.h"
#include "ln_nvds.h"

#include "utils/reboot_trace/reboot_trace.h"
#include "utils/debug/ln_assert.h"
#include "utils/debug/log.h"
#include "osal/osal.h"

#include "httpsclient.h"
#include "httpsclient_wrapper.h"
#include "httpsclient_debug.h"

#define    TAG  "OTAAPT"
#define    ZJ_LOCAL_LOG_LVL LOG_LVL_INFO

#undef ZJ_LOG
#define ZJ_LOG(fmt, ...)  LOG(ZJ_LOCAL_LOG_LVL, "["TAG"] "fmt, ##__VA_ARGS__)
#define LOG_EMSG ZJ_LOG

#define SECTOR_SIZE_4KB            (1024 * 4)

static bool g_ota_port_initialized = false;

/**
 * 下载错误，http 连接错误，g_ota_result = 2
 * 成功拿到 ota 的 header 的时候，g_ota_result = 0， g_progress_comp = 0
 * 进度更新的时候，g_ota_result = 0， g_progress_comp = 实际进度
 * 如果 version 检查失败，g_ota_result = 3， g_progress_comp = 100
 * 如果 header 解析失败，g_ota_result = 7， g_progress_comp = 100
 * 如果校验失败，g_ota_result = 6， g_progress_comp = 100
*/
static uint8_t g_ota_result = 0;
static uint8_t g_progress_comp = 0; // 只有进度有变化的时候才上报

int zj_ota_boot_addr_read(uint32_t *OTA_flash_start_addr)
{
    return 0;
}
int zj_ota_boot_update(uint32_t newAddr)
{
    return 0;
}
void zj_ota_erase_area(uint32_t boot_start_addr)
{
    return 0;
}
int zj_ota_write(uint32_t addr, uint8_t *dat,int len)
{
    return 0;
}

int zj_ota_write_end()
{
    return 0;
}

int zj_ota_read(uint32_t addr,uint8_t *dat,int len)
{
    return 0;
}


// a block to save http data.
static char *temp4K_buf    = NULL;
static int   temp4k_offset = 0;

// where to save OTA data in flash.
static int32_t flash_ota_start_addr  = OTA_SPACE_OFFSET;
static int32_t flash_ota_offset      = 0;
static uint8_t is_persistent_started = LN_FALSE;
static uint8_t is_ready_to_verify    = LN_FALSE;
static uint8_t is_precheck_ok        = LN_FALSE;
static OS_Thread_t g_httpota_thread;
static uint8_t httpc_ota_started = 0;

/**
 * @brief Pre-check the image file to be downloaded.
 *
 * @attention None
 *
 * @param[in]  app_offset  The offset of the APP partition in Flash.
 * @param[in]  ota_hdr     pointer to ota partition info struct.
 *
 * @return  whether the check is successful.
 * @retval  #LN_TRUE     successful.
 * @retval  #LN_FALSE    failed.
 */
static int ota_download_precheck(uint32_t app_offset, image_hdr_t * ota_hdr)
{
    image_hdr_t *app_hdr = NULL;
    if (NULL == (app_hdr = OS_Malloc(sizeof(image_hdr_t)))) {
        LOG(LOG_LVL_ERROR, "[%s:%d] malloc failed.\r\n", __func__, __LINE__);
        return LN_FALSE;
    }

    if ( OTA_ERR_NONE != image_header_fast_read(app_offset, app_hdr) ) {
        LOG(LOG_LVL_ERROR, "failed to read app header.\r\n");
        goto ret_err;
    }

    if ((ota_hdr->image_type == IMAGE_TYPE_ORIGINAL) || \
        (ota_hdr->image_type == IMAGE_TYPE_ORIGINAL_XZ)) 
    {
        // check version
        if (((ota_hdr->ver.ver_major << 8) + ota_hdr->ver.ver_minor) == \
            ((app_hdr->ver.ver_major << 8) + app_hdr->ver.ver_minor)) {
            LOG(LOG_LVL_ERROR, "[%s:%d] same version, do not upgrade!\r\n",
                    __func__, __LINE__);
            goto ret_err;
        }

        // check file size
        if (((ota_hdr->img_size_orig + sizeof(image_hdr_t)) > APP_SPACE_SIZE) || \
            ((ota_hdr->img_size_orig_xz + sizeof(image_hdr_t)) > OTA_SPACE_SIZE)) {
            LOG(LOG_LVL_ERROR, "[%s:%d] size check failed.\r\n", __func__, __LINE__);
            goto ret_err;
        }
    } else {
        //image type not support!
        goto ret_err;
    }

    OS_Free(app_hdr);
    return LN_TRUE;
    
    ret_err:
    OS_Free(app_hdr);
    return LN_FALSE;
}

static int ota_persistent_start(void)
{
    if ( NULL == temp4K_buf ) {
        temp4K_buf = OS_Malloc(SECTOR_SIZE_4KB);
        if ( NULL == temp4K_buf) {
            LOG_EMSG("failed to alloc 4KB!!!\r\n");
            return LN_FALSE;
        }
        memset(temp4K_buf, 0, SECTOR_SIZE_4KB);
    }

    temp4k_offset = 0;
    flash_ota_start_addr = OTA_SPACE_OFFSET;
    flash_ota_offset = 0;
    is_persistent_started = LN_TRUE;
    return LN_TRUE;
}

/**
 * @brief Save block to flash.
 *
 * @param buf
 * @param buf_len
 * @return return LN_TRUE on success, LN_FALSE on failure.
 */
static int ota_persistent_write(const char *buf, const int32_t buf_len)
{
    int part_len = 0; // [0, 1, 2, ..., 4K-1], 0, 1, 2, ..., (part_len-1)

    if (!is_persistent_started) {
        return LN_TRUE;
    }

    if (temp4k_offset + buf_len < SECTOR_SIZE_4KB) {
        // just copy all buf data to temp4K_buf
        memcpy(temp4K_buf + temp4k_offset, buf, buf_len);
        temp4k_offset += buf_len;
        part_len = 0;
    } else {
        // just copy part of buf to temp4K_buf
        part_len = temp4k_offset + buf_len - SECTOR_SIZE_4KB;
        memcpy(temp4K_buf + temp4k_offset, buf, buf_len - part_len);
        temp4k_offset += buf_len - part_len;
    }

    if (temp4k_offset >= (SECTOR_SIZE_4KB - 1) ) {
        // write to flash
        LOG(LOG_LVL_INFO, "write at flash: 0x%08x, offset:%d\r\n", flash_ota_start_addr + flash_ota_offset, flash_ota_offset);
        
        if(flash_ota_offset == 0) {
            if (LN_TRUE != ota_download_precheck(APP_SPACE_OFFSET, (image_hdr_t *) temp4K_buf)) {
                LOG(LOG_LVL_INFO, "ota download precheck failed!\r\n");
                is_precheck_ok = LN_FALSE;
                return LN_FALSE;
            }
            is_precheck_ok = LN_TRUE;
            LOG(LOG_LVL_INFO, "precheck ok\r\n");
        }
        
        LOG(LOG_LVL_INFO, "hal flash erase start\r\n");
        hal_flash_erase(flash_ota_start_addr + flash_ota_offset, SECTOR_SIZE_4KB);
        LOG(LOG_LVL_INFO, "hal flash program start\r\n");
        hal_flash_program(flash_ota_start_addr + flash_ota_offset, SECTOR_SIZE_4KB, (uint8_t *)temp4K_buf);

        flash_ota_offset += SECTOR_SIZE_4KB;
        memset(temp4K_buf, 0, SECTOR_SIZE_4KB);
        temp4k_offset = 0;
    }

    if (part_len > 0) {
        memcpy(temp4K_buf + temp4k_offset, buf + (buf_len - part_len), part_len);
        temp4k_offset += part_len;
    }

    return LN_TRUE;
}

/**
 * @brief save last block and clear flags.
 * @return return LN_TRUE on success, LN_FALSE on failure.
 */
static int ota_persistent_finish(void)
{
    if (!is_persistent_started) {
        return LN_FALSE;
    }

    // write to flash
    LOG(LOG_LVL_INFO, "write at flash: 0x%08x\r\n", flash_ota_start_addr + flash_ota_offset);
    hal_flash_erase(flash_ota_start_addr + flash_ota_offset, SECTOR_SIZE_4KB);
    hal_flash_program(flash_ota_start_addr + flash_ota_offset, SECTOR_SIZE_4KB, (uint8_t *)temp4K_buf);

    OS_Free(temp4K_buf);
    temp4K_buf = NULL;
    temp4k_offset = 0;

    flash_ota_offset = 0;
    is_persistent_started = LN_FALSE;
    return LN_TRUE;
}

static int http_get_callback(const char *recvbuf, int32_t recvbuf_len, int32_t total_bytes, int8_t is_recv_finished)
{
    uint8_t progress = 0;
    static uint32_t tmp_received_len = 0;
    // LOG_I("recv len:%d, total:%d, finish:%d\r\n", recvbuf_len, total_bytes, is_recv_finished);
    if (!is_recv_finished) {
        if (LN_TRUE != ota_persistent_write(recvbuf, recvbuf_len)) {
            if (!is_precheck_ok) {
                g_ota_result = 3;
                zj_adapter_post_event(ADAPT_EVT_OTA_STATUS, &g_ota_result,NULL, 100);
            }
            return -1;
        } else {
            tmp_received_len += recvbuf_len;
            progress = tmp_received_len * 100 / total_bytes;
            if (progress % 5 == 0 && g_progress_comp != progress) {
                g_progress_comp = progress;
                g_ota_result = 0;
                // LOG_I("Before post event\r\n");
                zj_adapter_post_event(ADAPT_EVT_OTA_STATUS, &g_ota_result, NULL, progress);
                // LOG_I("==== ota progress:%d%% ====\r\n", progress);
            }
        }
    } else {
        ota_persistent_finish();
        is_ready_to_verify = LN_TRUE;
        LOG_I("cb info: recv %d finished, no more data to deal with.\r\n", total_bytes);
    }
    return recvbuf_len;
}

/**
 * @brief check ota image header, body.
 * @return return LN_TRUE on success, LN_FALSE on failure.
 */
static int ota_verify_download(void)
{
    image_hdr_t ota_header;

    if ( OTA_ERR_NONE != image_header_fast_read(OTA_SPACE_OFFSET, &ota_header) ) {
        LOG(LOG_LVL_ERROR, "failed to read ota header.\r\n");
        return LN_FALSE;
    }

    if ( OTA_ERR_NONE != image_header_verify(&ota_header) ) {
        LOG(LOG_LVL_ERROR, "failed to verify ota header.\r\n");
        return LN_FALSE;
    }

    if ( OTA_ERR_NONE != image_body_verify(OTA_SPACE_OFFSET, &ota_header)) {
        LOG(LOG_LVL_ERROR, "failed to verify ota body.\r\n");
        return LN_FALSE;
    }

    LOG(LOG_LVL_INFO, "Succeed to verify OTA image content.\r\n");
    return LN_TRUE;
}

static int update_ota_state(void)
{
    upg_state_t state = UPG_STATE_DOWNLOAD_OK;
    ln_nvds_set_ota_upg_state(state);
    return LN_TRUE;
}


/** 
 * @brief 开启在线更新
 * @note  
 * @param[in] http_url:url
 * @return 0:success -1:fail
 */
int zj_ota_get_image(char *http_url)
{
    int ret_code = 0;
    HTTP_INFO *http_info = NULL;
    char *url_str = NULL;
    int recv_total = 0;
    ota_port_ctx_t * ota_port = NULL;

    zj_ble_scan_stop();

    if (!g_ota_port_initialized) {
        if ( OTA_ERR_NONE != ota_port_init()) {
            LOG(LOG_LVL_ERROR, "ota port failed!\r\n");
            return -1;
        }
    }

    ota_port = ota_get_port_ctx();

    while(!netdev_got_ip()){
        OS_MsDelay(500);
    }

    ZJ_LOG("http start <%s>.!!!\r\n", http_url);
    while(1) {
        if (NULL == (http_info = httpinfo_new())) {
            LOG_EMSG("malloc for @http_info failed.\r\n");
            ret_code = -1;
            goto flag_exit;
        }

        if (0 != http_init(&http_info, FALSE)) {
            LOG_EMSG("http init failed.!!!\r\n");
            ret_code = -1;
            goto flag_err;
        }

        url_str = http_url;

        if (LN_TRUE == ota_persistent_start()) {
            recv_total = http_get_with_callback(http_info, url_str, http_get_callback);
        }

        if (recv_total > 0) {
            LOG_I("total received %d bytes.\r\n", recv_total);
            ret_code = 0;
        }
        else if((recv_total < 0) && (is_precheck_ok != LN_TRUE)) {
            ret_code = -2;
        } 
        else {
            ret_code = -1;
            LOG_EMSG("http_get error!!!\r\n");
        }

        http_close(http_info);

flag_err:
        httpinfo_delete(http_info);

flag_exit:

        OS_SecDelay(5);
        if (ret_code != 0) {
            if (ret_code == -2) {
                break;
            } else {
                // retry if something is wrong.
                LOG_EMSG("http client is restart...\r\n");
            }
        } else {
            // just download once if everything is ok.
            break;
        }
    }

    LOG_IMSG("http client job done, exit...\r\n");
    if (LN_TRUE == is_precheck_ok)
    {
        if ( (LN_TRUE == is_ready_to_verify) && (LN_TRUE == ota_verify_download()) ) {
            g_ota_result = 0;
            zj_adapter_post_event(ADAPT_EVT_OTA_STATUS, &g_ota_result, NULL, 100);

            update_ota_state();
            ln_chip_reboot();
            while(1);
        } else {
            g_ota_result = 6;
            zj_adapter_post_event(ADAPT_EVT_OTA_STATUS, &g_ota_result, NULL, 100);
        }
    }

    httpc_ota_started = 0;
    return -1;
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
    ln_chip_reboot();
    while(1);
}
