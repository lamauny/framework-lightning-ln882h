#ifndef ZG_REMOTE_H
#define ZG_REMOTE_H

#include "version_config.h"

void light_remote24k_handler(uint8_t key);
void light_remote44k_handler(uint8_t key);
void light_remote_init();
int pub_h2_rmt_handle(btr_arg_t *arg);
int pub_remote_event_cb(btr_arg_t *arg);
void btr_bind_indicator(uint8_t light_time,uint8_t dark_time,uint8_t times,uint8_t trans_time);
void remote_astrict_cus_bringhtness(uint8_t *execution_type,uint8_t *br_rgb,uint8_t *br_wc);

#define BT_KEY_SPBR_ADD_SHORD        0x01
#define BT_KEY_SPBR_ADD_LONG         0xFE

#define BT_KEY_SPBR_CUS_SHORD        0x02
#define BT_KEY_SPBR_CUS_LONG         0xFD

#define BT_KEY_SPEED_ADD_SHORD       0x0C
#define BT_KEY_SPEED_ADD_LONG        0xF3

#define BT_KEY_SPEED_CUS_SHORD       0x0D
#define BT_KEY_SPEED_CUS_LONG        0xF2

#define BT_KEY_MODE_ADD_SHORD        0x0E
#define BT_KEY_MODE_ADD_LONG         0xF1

#define BT_KEY_MODE_CUS_SHORD        0x0F
#define BT_KEY_MODE_CUS_LONG         0xF0

#define BT_KEY_AUTO_SHORD            0x2A
#define BT_KEY_AUTO_LONG             0xD5

#define BT_KEY_BR_ADD_SHORD          0x2B
#define BT_KEY_BR_ADD_LONG           0xD4

#define BT_KEY_BR_CUS_SHORD          0x2C
#define BT_KEY_BR_CUS_LONG           0xD3

#define BT_KEY_MIC_FADE_SHORD        0x2D
#define BT_KEY_MIC_FADE_LONG         0xD2

#define BT_KEY_MIC_JUMP_SHORD        0x2E
#define BT_KEY_MIC_JUMP_LONG         0xD1

#define BT_KEY_MIC_STROBE_SHORD      0x2F
#define BT_KEY_MIC_STROBE_LONG       0xD0

#define BT_KEY_LOOP_SHORD            0x30
#define BT_KEY_LOOP_LONG             0xCF

#define BT_KEY_STATIC_MOVE_UP        0x67
#define BT_KEY_STATIC_MOVE_DOWN      0x68

#define BT_KEY_MS_CHANGE             0xFB 
#define BT_KEY_STATIC_COLOR          0x6D

#define BT_KEY_CONFIG_NET            0xDB

#define BT_KEY_REMOTE_TEST           0xFF

#define BT_KEY_COLOR_SIXTEEN         0X24
#define BT_KEY_LOCK_REMOTE           0XD1

#define BT_KEY_NIGHT_LAMP_SHORD		 0x24
#define BT_KEY_NIGHT_LAMP_LONG		 0xDB
#define BT_KEY_NIGHT_LAMP_REMOTE	 0x0A

#endif