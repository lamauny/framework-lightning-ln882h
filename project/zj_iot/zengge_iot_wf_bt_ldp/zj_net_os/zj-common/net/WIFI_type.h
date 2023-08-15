#ifndef ZJ_WIFI_TYPE
#define ZJ_WIFI_TYPE

typedef enum{

    ZG_MODE_NONE = 0,
	ZG_STA_MODE = 1,
	ZG_AP_MODE = 2,
    ZG_MODE_BUTT
}ZG_wifi_mode_t;

typedef struct wifi_info_tt
{
   unsigned char ssid[33];
   unsigned char method;
   unsigned char ssid_len;
   unsigned char pwd[65];
   unsigned char pwd_len;
}wifi_info_t;

#endif