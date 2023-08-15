#ifndef __BACKWARD_COMPATIBILITY
#define __BACKWARD_COMPATIBILITY

#include "ZG_system.h"

#define ZG_event_loop_init           register_zjos_event
#define ZG_regist_data_event_cb      register_app_data_event
#define register_wifi_app_data_event register_app_data_event
#define def_cfg_register_event       register_def_cfg_event
#define ptest_register_event         register_ptest_event 
#define ZG_event_send                zjos_event_post

#define ZG_set_app_icon_id         zj_set_app_icon_id
#define ZG_get_app_icon_id         zj_get_app_icon_id
#define ZG_storage_module_start    zj_storage_module_start
#define ZG_monitor_start           zj_monitor_start
#define ZG_creat_device_id         zj_creat_device_mid
#define ZG_device_info_conf        zj_device_info_conf
#define ZG_device_version_conf     zj_device_version_conf 
#define ZG_device_mcu_ver_conf     zj_device_mcu_ver_conf 

#define ZG_get_fw_version               zj_get_fw_version
#define ZG_get_fw_version_hex           zj_get_fw_version_hex
#define ZG_get_device_mid               zj_get_device_mid
#define ZG_get_device_mid_version       zj_get_device_mid_version 
#define ZG_get_mcu_version              zj_get_mcu_version
#define ZG_get_device_code              zj_get_device_code
#define ZG_get_device_protocol_version  zj_get_device_protocol_version
#define ZG_get_chip_type                zj_get_chip_type


#define  zj_memcpy     memcpy
#define  zj_memcmp     memcmp
#define  zj_memset     memset
#define  MEMCMP_OK     0

#endif