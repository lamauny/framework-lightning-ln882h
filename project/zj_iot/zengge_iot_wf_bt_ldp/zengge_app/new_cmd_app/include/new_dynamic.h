#ifndef _NEW_DYNAMIC__
#define _NEW_DYNAMIC__

#include "light_main.h"

typedef enum{

    EXECUTION_DYNAMIC_BREATHING = 1,
    EXECUTION_DYNAMIC_STROBE,
    EXECUTION_DYNAMIC_FADE,
    EXECUTION_DYNAMIC_JUMP
}dynamic_mode_type_t;

typedef struct{

    dynamic_mode_type_t dynamic_type;
    execution_type_t    color_type;   //EXECUTION_HSV or EXECUTION_CCT_BRIGHTNESS
    uint16_t val1;
    uint8_t val2;
    uint8_t br;
    uint8_t execution_number;
    uint16_t effect_time;
    uint16_t blink_time;
    uint16_t rand_max_time;
}custom_mode_arg_t;

int dynamic_core_breath(custom_mode_arg_t arg);
int dynamic_core_jump(custom_mode_arg_t arg);
int dynamic_core_strobe(custom_mode_arg_t arg);
int dynamic_core_rgb_fade(custom_mode_arg_t arg1,custom_mode_arg_t arg2);
int dynamic_core_cct_fade(custom_mode_arg_t arg1,custom_mode_arg_t arg2);

bool custom_dynamic_handler(uint8_t step,custom_mode_arg_t *arg);

void light_test_mode(void);
void light_rgbw_mode(void); //测试模式
void light_cct_test_mode(void); //测试模式
void light_warm_test_mode(void); //测试模式
void light_aging_mode();
void light_cct_aging_mode();
void light_warm_aging_mode();
void light_remote_test_mode(void);
void light_reset_mode(void);
void light_cct_reset_mode(void) ;
void light_controller_rgb_jump(void) 	;
void light_controller_rgbw_jump(void) 	;
void light_controller_rgbwc_jump(void) ;
void new_built_in_mode_change(uint8_t rmode);

void new_custom_mode_get_cmd(uint8_t *data);
void new_custom_mode_parse_cmd(uint8_t *data,int len);
void new_custom_mode_init();

void new_scene_mode_handler(uint8_t mode);
uint16_t scene_speed();

void new_dynamic_mode_handler();
void new_dynamic_param_init();

void preview_task_init();
void new_preview_cancle();
void new_preview_record();
void new_preview_exit2previous_state();
void preview_done_handle();

void fixed_scene_br_convert(uint8_t mode,uint8_t *br);

#endif