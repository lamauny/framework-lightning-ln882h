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
#include "ln_ble_rw_app_task.h"
#include "ln_ble_gap.h"
#include "ln_ble_gatt.h"
#include "ln_ble_gap_ind_handler.h"
#include "ln_ble_gatt_ind_handler.h"
#include "ln_ble_trans_server.h"
#include "ln_ble_app_err_code.h"
#include "ln_ble_app_default_cfg.h"
#include "ln_ble_app_defines.h"
#include "gapm_task.h"

void _char_cccd_enable_set(uint8_t en);
uint8_t _cccd_handle_get(void);
uint8_t _char_notify_handle_get(void);
uint8_t _char_write_handle_get(void);
uint8_t _is_char_write_handle(uint8_t hdl);
uint8_t _is_char_cccd_handle(uint8_t hdl);
void _char_svc_handle_set(uint8_t start_hdl);

void creat_usr_app_task(void);


#ifdef __cplusplus
}
#endif

#endif /* __USR_APP_H__ */


