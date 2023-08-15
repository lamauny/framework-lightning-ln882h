#ifndef __AC_MONITOR_H
#define __AC_MONITOR_H


#include "version_config.h"
#ifdef APPCONFIG_SUPPORT_AC_MONITOR



#define  MONITOR_GPIO_INPUT  	PORT_CONFIG_HV_WAVE_GPIO     
#define  HV_RESTORE_STORE  		"HV_RESTORE"


typedef enum{
	AC_RESET_POWERON = 0,
	AC_RESET_COUNT
}ac_reset_type_t;

void ac_monitor_init();


#endif


#endif




