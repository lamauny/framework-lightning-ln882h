#ifndef __USR_APP_H__
#define __USR_APP_H__

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

#include "wifi.h"

typedef struct Link
{
    ap_info_t data;
    struct Link *next;
}app_sta_scan_data_t;

typedef struct
{
    uint8_t data_fetech;
    uint8_t channel_num;
    uint16_t channels[14];
    uint8_t bssid[7];
    char ssid[65];
    uint8_t scan_mode;
    uint32_t duration_scan;
    uint8_t filter_mask;
    app_sta_scan_data_t *data;
}app_sta_scan_cfg_t;

extern app_sta_scan_cfg_t g_sta_scan_cfg;

void creat_usr_app_task(void);
int wifi_mgmr_scan_adv(void *data, void (*cb)(void *arg), uint16_t *channels, uint16_t channel_num, uint8_t bssid[6], char *ssid, uint8_t scan_mode, uint32_t duration_scan);



#ifdef __cplusplus
}
#endif

#endif /* __USR_APP_H__ */


