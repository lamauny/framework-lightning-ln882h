#ifndef FOTA_H_
#define FOTA_H_


#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UPGRADE_FAIL = -1,
    UPGRADE_PROGRESS = 0,
    UPGRADE_VER_ERR,
    UPGRADE_HTTP_FAIL,
    UPGRADE_TYPE_ERR,
    UPGRADE_ONGIONG,
    UPGRADE_AES_ERR,
    UPGRADE_MD5_ERR,
    UPGRADE_HEADER_ERR,
    UPGRADE_TIMEOUT,
    UPGRADE_SUCCESS = 100,
    UPGRADE_FILE_COMPLETE,
}fota_status_t;  

#define  ERR_MSG_NULL    NULL
#define  ERR_MSG_TIMEOUT "http timeout"


typedef enum 
{
  OTA_TRIGGER_BY_AT,
  OTA_TRIGGER_BY_CLOUD	
}ota_from_t;

void fota_set_status(fota_status_t status, uint8_t progress,char *msg);
void fota_update_link(char *pdata,ota_from_t type);
bool fota_check_start_status();
void ota_running_flag_set();
int fota_get_progress(uint32_t time,uint8_t *progress);
bool fota_get_noLight_ind_flag();
int fota_complete_processing();
bool ota_hear_parse(uint8_t key,uint8_t *inData,int inData_len,uint8_t *verCheck_flag,uint8_t *fw_enc_flag);
bool fota_is_wifi_need_upgrade();
bool fota_is_tlk_chip_need_upgrade();
uint32_t fota_get_wifi_bin_size();
uint32_t fota_get_slave_bin_size();

#ifdef __cplusplus
}
#endif



#endif 

