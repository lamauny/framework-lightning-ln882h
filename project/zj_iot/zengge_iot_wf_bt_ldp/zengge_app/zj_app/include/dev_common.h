#ifndef _IRRGB_UTILS_H_
#define _IRRGB_UTILS_H_

#include "version_config.h"
#include "light_clock.h"
#include "light_main.h"
#include "light_remote.h"
#include "light_dynamic.h"
#include "dev_key.h"
#include "dev_command_head.h"
#include "key_handle.h"
#include "new_cmd_common.h"
#include "new_wake_sleep.h"
#include "new_light_family.h"
#include "iic_drv_SM2135Ex.h"
#include "iic_drv_SM2235Ex.h"
#include "iic_drv_BP5758D.h"
#include "simulateIIC.h"
#include "light_mic.h"
#include "light_touch.h"
#include "pwr_opt_record.h"
#include "zj_ir.h"
#include "light_ac_monitor.h"
#include "light_night_lamp.h"


void dev_report_init();
void ZG_system_event_init();
void ZG_data_event_init();
void task_status_monitor_init();
void ZG_light_system_start(void);
void light_dynamic_sync_init();
void light_dynamic_sync_cmd_send();

#endif


