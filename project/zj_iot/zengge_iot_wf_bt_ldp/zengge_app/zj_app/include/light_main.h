#ifndef _IRLIGHTFUNC__
#define _IRLIGHTFUNC__

#include "ZG_system.h"


#define ZG_LIGHT_TASK_PRIO                 PORT_CONFIG_LIGHT_TASK_PRIO
#define ZG_LIGHT_TASK_STACK_SZ             (768)

#define ZG_LIGHT_REMOTE_TASK_PRIO          PORT_CONFIG_LIGHT_REMOTE_TASK_PRIO
#define ZG_LIGHT_REMOTE_TASK_STACK_SZ      (768)

#define ZG_TIMER_CLOCK_TASK_PRIO           PORT_CONFIG_TIMER_CLOCK_TASK_PRIO
#define ZG_TIMER_CLOCK_TASK_STACK_SZ       (512)

#define ZG_AUTO_REPORT_TASK_PRIO           PORT_CONFIG_AUTO_REPORT_TASK_PRIO
#define ZG_AUTO_REPORT_TASK_STACK_SZ       (768)

#define ZG_LIGHT_MIC_TASK_PRIO             PORT_CONFIG_MIC_TASK_PRIO
#define ZG_LIGHT_MIC_TASK_STACK_SZ         (512)

#define ZG_LIGHT_KEY_TASK_PRIO             PORT_CONFIG_KEY_TASK_PRIO     

#define STORE_PER_TIME        3
#define LIGHT_DBG  printf

typedef enum{

  EXECUTION_SET_BRIGHTNESS     = 0x01,            /*代表设置亮度, 其他属性保持不变*/
  EXECUTION_ADD_BRIGHTNESS     = 0x02,            /*代表设置增加亮度, 其他属性不变*/
  EXECUTION_CUS_BRIGHTNESS     = 0x03,            /*代表设置减少亮度, 其他属性不变*/
  EXECUTION_HSV                = 0xA1,            /*代表设置 静态HSV,如果在light_color_covert调用:设置完自动变0x61静态模式*/
  EXECUTION_HS                 = 0XA2,            /*代表设置 静态HS,如果在light_color_covert调用:设置完自动变0x61静态模式*/
  EXECUTION_CCT_BRIGHTNESS     = 0xB1,            /*代表设置 静态CCT + 亮度,如果在light_color_covert调用:设置完自动变0x61静态模式*/
  EXECUTION_SET_CCT            = 0XB2,            /*代表设置 静态CCT，如果在light_color_covert调用:亮度保持不变,设置完自动变0x61静态模式*/
  EXECUTION_AUX_CCT_BRIGHTNESS = 0XB3,            /*代表设置 CCT + 亮度+辅助光*/
  EXECUTION_AUX                = 0xB4,            /*代表设置 辅助光，CCT及亮度保持不变*/
  EXECUTION_ADD_CCT            = 0xB5,            /*代表增加CCT，亮度保持不变*/
  EXECUTION_CUS_CCT            = 0xB6,            /*代表减少CCT，亮度保持不变*/
  EXECUTION_HSV_CCT            = 0xC1,            /*代表设置 HSV、CCT*/
  EXECUTION_HS_CCT             = 0XC2,            /*代表设置 HS, CCT，亮度保持不变*/
  EXECUTION_HSV_CCT_HOLD       = 0XC3,            /*代表设置 HSV, CCT保持不变*/
  EXECUTION_CCT_HSV_HOLD       = 0XC4,            /*代表设置 CCT, HSV保持不变*/
  
  EXECUTION_RGB                = 0XD1,            /*代表设置RGB,如果在light_color_covert调用:设置完自动更新HSV值,自动变0x61静态模式*/
  EXECUTION_RGB_NOBR           = 0XD2,            /*代表设置RGB,亮度不变,设置完自动更新HS值,自动变0x61静态模式*/
  EXECUTION_RGBWC_DYNAMIC      = 0xD3,            /*代表设置动态模式的RGB值*/
  EXECUTION_WC                 = 0xD4,            /*代表设置warm cold,设置完自动更新cct,v值,如果在light_color_covert调用:自动变0x61静态模式*/
  EXECUTION_RGBWC_STATIC       = 0xD5,            /*代表设置rgb warm cold,设置完自动更新cct,HSV值,如果在light_color_covert调用:自动变0x61静态模式*/

  EXECUTION_LIGHT_ON           = 0X23,            /*代表设置 开灯*/
  EXECUTION_LIGHT_OFF          = 0x24,            /*代表设置 关灯*/
  EXECUTION_LIGHT_ONOFF        = 0x25,            /*代表设置 开关取反*/
  EXECUTION_DELAY_OFF          = 0x73,
	EXECUTION_GRADIENT_OFF       = 0x75
}execution_type_t;

typedef struct{
   
   execution_type_t execution_type;      /*执行类型*/
   uint16_t h;                          
   uint8_t s;
   uint8_t rgb_v;
   uint8_t red;  //执行类型是D1 D2 D3时才有效
   uint8_t green;//执行类型是D1 D2 D3时才有效
   uint8_t blue; //执行类型是D1 D2 D3时才有效
   uint8_t warm; //执行类型是D3 D4 时才有效
   uint8_t cold; //执行类型是D3 D4 时才有效
   uint8_t cct_or_type;
   uint8_t cct_v;
   uint16_t delay_time;
   uint32_t transition_time;
}color_convert_t;

typedef enum{
 
  MUSIC_MODE_BREATH,
  MUSIC_MODE_FADE,
  MUSIC_MODE_JUMP,
  MUSIC_MODE_STROBE
}music_type_t;

typedef enum {
    R_SORT = 1,
    G_SORT,
    B_SORT,
    W_SORT,
    C_SORT,
    MAX_SORT
} sort_t;

typedef struct
{
    unsigned char R_ch;
    unsigned char G_ch;
    unsigned char B_ch;
    unsigned char W_ch;
    unsigned char C_ch;
} color_channel_t;

typedef enum {
    DSP_RGBCCT = 0x5A,
    DSP_RGB    = 0xF0,
    DSP_CCT    = 0x0F
} color_type_t;

typedef struct 
{
    uint16_t h;
    uint8_t sat;
    uint8_t cct;
}hs_cct_t;

typedef struct
{
  hs_cct_t hscct;
  color_type_t color_show_type;
  uint8_t mode_type;
  uint8_t running_mode;
  uint8_t speed;
  uint8_t sw;
  uint8_t rgb_v;
  uint8_t cct_v;
}light_family_t;

typedef enum{
  
  MBR_CFG_RGBBR,
  MBR_CFG_CCTBR,
  MBR_CFG_BOTHBR,
  MBR_CFG_MODE,
  MBR_CFG_BOTHBR_MODE,
  MBR_CFG_RGBBR_MODE,
  MBR_CFG_CCTBR_MODE
}mbr_cfg_type_t;

typedef struct{
   
   mbr_cfg_type_t cfg_type;
   uint8_t rgb_br;
   uint8_t cct_br;
   uint8_t mode_type;
}mbr_cfg_t;


#define      POWER_ON         0x23
#define      POWER_OFF        0x24
#define      POWER_ON_OFF     0x25

#define      BLINK_ALWAYS_ON  0xFFFF

#ifndef LIGHT_CORE_DL_TIME
#define LIGHT_CORE_DL_TIME      6
#endif

enum {

    MODE_TYPE_RGB         = 0x39,
    MODE_TYPE_RESET       = 0x64,
    MODE_TYPE_FACTORY     = 0x63,
    MODE_TYPE_REMOTE_TEST = 0xFE,
    MODE_TYPE_TEST        = 0xFF,
    MODE_TYPE_ROUTER      = 0x66,

    MODE_TYPE_SUNRISE     = 0xA1,
    MODE_TYPE_SUNSET      = 0xA2,
    MODE_TYPE_USER        = 0x60,
    MODE_TYPE_STATIC      = 0x61,
    MODE_TYPE_MUSIC       = 0x62,
    MODE_TYPE_CANDLE      = 0x5F,
    MODE_TYPE_BLINK       = 0x5E,
    MODE_TYPE_MIC         = 0x5D,
    MODE_TYPE_MIC_TEST    = 0x5C,
    MODE_TYPE_LOOP        = 0x5B,
    MODE_TYPE_FIX_SCENE   = 0x70,
    MODE_TYPE_CUSTOM_NEW  = 0x71
};

#define DYNAMIC_SPEED_MIN      0x01
#define DYNAMIC_SPEED_MAX      0x64
#define DYNAMIC_SPEED_DEFAULT  0x40

void light_set_speed(unsigned char speed);
unsigned char light_get_speed();

void light_set_mode_type(unsigned char mode);
unsigned char light_get_mode_type();
void light_record_mode_type(uint8_t mode);

void light_set_power(unsigned char sw,uint32_t transition_time);
unsigned char light_get_power();

void light_set_wiring_type(uint8_t option);
uint8_t light_get_wiring_type();
bool is_light_support_both_on();
void light_preview_color_sort_cancle();
void light_preview_color_sort(sort_t ch1_R,sort_t ch2_G,sort_t ch3_B,sort_t ch4_W,sort_t ch5_C);
void light_set_color_sort(sort_t ch1_sort,sort_t ch2_sort,sort_t ch3_sort,sort_t ch4_sort,sort_t ch5_sort);

void light_set_running_mode(unsigned char mode);
unsigned char light_get_running_mode();

void light_thread_init();

void light_task_change_priority(bool set_higher);
TaskHandle_t light_get_task_handler(void);

void light_color_covert(color_convert_t *para);

int light_check_color_change_flag(int xTicksToWait);

void light_set_rgb_lightness(uint8_t br,uint32_t time);

uint8_t light_get_target_rgb_lightness();
uint16_t light_get_target_hue();
uint8_t light_get_target_sat();

uint16_t light_get_current_hue();
uint8_t  light_get_current_sat();
uint8_t light_get_current_rgb_lightness();

void light_set_cct_lightness(uint8_t br,uint32_t time);
uint8_t light_get_target_cct_lightness();
uint8_t light_get_target_cct();

uint8_t light_get_curren_cct();
uint16_t light_get_current_cct_lightness();

unsigned char light_get_color_flag();
unsigned char light_get_current_red();
unsigned char light_get_current_green();
unsigned char light_get_current_blue();
unsigned char light_get_current_warm();
unsigned char light_get_current_cold();

void light_get_led_family(light_family_t *fmy);

void light_cct_covert_wc(uint8_t temp,uint8_t br,uint8_t *w,uint8_t *c);

void light_music_set_color(color_convert_t *color);
void light_music_init();
void light_music_set_type(music_type_t type);
music_type_t light_music_get_type();
void  light_music_mode_handle();

uint16_t light_get_countdown_remain();
bool light_check_countdown_flag();
bool light_check_countdown_update_flag();

void light_old_dynamic_mode_handle(color_convert_t *color_out);

void light_get_oldPro_dev_status(uint8_t *out_data);
void light_get_oldRep_dev_status(uint8_t *out_data);
void light_get_newPro_dev_status(uint8_t *out_data,uint8_t *out_len);
void light_set_mbr_config(mbr_cfg_t cfg,uint32_t time);

#endif
