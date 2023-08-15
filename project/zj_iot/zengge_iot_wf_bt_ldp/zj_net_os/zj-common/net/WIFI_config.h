#ifndef _WIFI_CONFIG_H_
#define _WIFI_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "WIFI_type.h"
#include "zj_adapt_api.h"

typedef enum{

  SYSTEM_NOMAL,
  SYSTEM_RESTORE
}system_status_t;
	
typedef enum{
	RESET_BY_POWER,
	RESET_BY_CMD
}reset_type_t;

typedef enum{

   SWITCH_THREE_TIMES_RESET_OFF,
   SWITCH_THREE_TIMES_RESET_ON
}reset_option_t;

void zj_wifi_ssid_conf(uint8_t* str,int len);
void zj_wifi_password_conf(uint8_t *str,int len);
void zj_wifi_Adapter_start();
void zj_wifi_Adapter_stop();
void factory_ap_conf(void);

void zj_set_reset_factory_number(uint8_t number);
system_status_t Restore_factory_settings_func(reset_type_t type,reset_option_t option);
system_status_t ZG_is_system_recover_net();

void zj_wifi_read_init(void);

void zj_wifi_set_mode(ZG_wifi_mode_t mode);
ZG_wifi_mode_t zj_wifi_get_mode();

void zj_wifi_get_conf_msg(wifi_info_t *wifi_msg);

void zj_wifi_scan_ap();
void zj_wifi_sta_restart();;

int zj_get_wifi_connection_status(int xTicksToWait);


bool is_wifi_cfg_in_progress();
void clear_wifi_cfg_flag();

#define get_ap_rssi()                             zj_get_ap_rssi()
#define get_wifi_connection_status(xTicksToWait)  zj_get_wifi_connection_status(xTicksToWait)
#define wifi_get_ip_info(ip)                      zj_wifi_get_ip_info(ip)
#define wifi_get_mac_info(mac_str)                zj_wifi_get_mac_info(mac_str)

#ifdef __cplusplus
}
#endif

#endif /* _COMMAND_H_ */
