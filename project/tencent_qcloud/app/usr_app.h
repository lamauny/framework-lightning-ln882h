#ifndef __USR_APP_H__
#define __USR_APP_H__

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

#define _FIMRWARE_SDK_V_1_8_

#include "ln_ble_gap.h"
#include "ln_ble_gatt.h"
#include "ln_ble_connection_manager.h"
#include "ln_ble_event_manager.h"
#include "ln_ble_smp.h"
#include "ln_ble_advertising.h"

void creat_usr_app_task(void);


#ifdef __cplusplus
}
#endif

#endif /* __USR_APP_H__ */


