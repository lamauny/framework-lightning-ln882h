#ifndef _LIGHT_DYNAMIC__
#define _LIGHT_DYNAMIC__

#include "light_main.h"


typedef enum {

    CANDLE_RANGE_SMALL = 1,
    CANDLE_RANGE_MIDDLE,
    CANDLE_RANGE_HIGN,
}candle_range_type_t;

typedef struct{
   
   execution_type_t execution_type;      /*执行类型*/                       
   uint8_t val_body[3];
   candle_range_type_t range;
}candle_conf_t;

typedef enum{

	DONE_LIGHT_OFF = 1,
	DONE_LIGHT_ORIGINAL,
	DONE_LIGHT_NEW
}blink_done_status_t;

typedef struct {

   execution_type_t execution_type;      /*执行类型*/                       
   uint8_t val_body[3];
   uint8_t light_time;
   uint8_t dark_time;
   uint16_t loop_number;
   uint8_t transition_effect;           /*渐变效果等级*/
   blink_done_status_t done_status;
}blink_conf_t;


void dynamic_candle_mode_init();
void dynamic_candle_mode_set_config(candle_conf_t *Parameters);
void dynamic_candle_mode_get_config(candle_conf_t *Parameters);
void dynamic_candle_loop_handler();

void dynamic_blink_mode_set_config(blink_conf_t *Parameters);
void dynamic_blink_loop_handler();

void old_dynamic_mode_handler();

void old_custom_init();
void old_cutom_set_config(uint8_t *dat);
void old_cutom_get_config(uint8_t *data);

#endif