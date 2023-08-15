#include "zj_adapt_api.h"
#include "zj_adapt_config.h"

#include "driver/ledc.h"
#include "esp_err.h"

#ifndef CONFIG_ZG_LED_RED_GPIO
#define CONFIG_ZG_LED_RED_GPIO -1
#endif

#ifndef CONFIG_ZG_LED_GREEN_GPIO
#define CONFIG_ZG_LED_GREEN_GPIO -1
#endif

#ifndef CONFIG_ZG_LED_BLUE_GPIO
#define CONFIG_ZG_LED_BLUE_GPIO -1
#endif

#ifndef CONFIG_ZG_LED_WARM_GPIO
#define CONFIG_ZG_LED_WARM_GPIO -1
#endif

#ifndef CONFIG_ZG_LED_COLD_GPIO
#define CONFIG_ZG_LED_COLD_GPIO -1
#endif

void zj_pwm_red_channel_init(int freq)
{
	ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_HIGH_SPEED_MODE,
        .channel        = LEDC_CHANNEL_0,
        .timer_sel      = LEDC_TIMER_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = CONFIG_ZG_LED_RED_GPIO,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void zj_pwm_green_channel_init(int freq)
{
	ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_HIGH_SPEED_MODE,
        .channel        = LEDC_CHANNEL_1,
        .timer_sel      = LEDC_TIMER_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = CONFIG_ZG_LED_GREEN_GPIO,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void zj_pwm_blue_channel_init(int freq)
{
	ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_HIGH_SPEED_MODE,
        .channel        = LEDC_CHANNEL_2,
        .timer_sel      = LEDC_TIMER_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = CONFIG_ZG_LED_BLUE_GPIO,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void zj_pwm_warm_channel_init(int freq)
{
	ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_HIGH_SPEED_MODE,
        .channel        = LEDC_CHANNEL_3,
        .timer_sel      = LEDC_TIMER_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = CONFIG_ZG_LED_WARM_GPIO,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void zj_pwm_cold_channel_init(int freq)
{
	ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_HIGH_SPEED_MODE,
        .channel        = LEDC_CHANNEL_4,
        .timer_sel      = LEDC_TIMER_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = CONFIG_ZG_LED_COLD_GPIO,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void zj_pwm_init(int freq)
{
	// Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_HIGH_SPEED_MODE,
        .timer_num        = LEDC_TIMER_0,
        .duty_resolution  = LEDC_TIMER_13_BIT,
        .freq_hz          = freq,  // Set output frequency at 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));
}
/**
 * @brief pwm set duty
 * @note 
 * @param[in] ch  : channel
 * @param[in] duty  : 0~25500
 * @return none
 */
void zj_pwm_set_duty(uint8_t ch,int duty)
{
	if(duty == 25500){

		duty = 8192;
	}else{
		duty = duty / 3.11279;
	}
	ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, ch, duty));
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, ch));
}
