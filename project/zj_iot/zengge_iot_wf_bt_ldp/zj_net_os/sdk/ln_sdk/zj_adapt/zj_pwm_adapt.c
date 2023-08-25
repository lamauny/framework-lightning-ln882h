#include "zj_adapt_api.h"
#include "zj_adapt_config.h"

#include "ln_drv_pwm.h"

#undef TAG
#undef ZJ_LOG
#define TAG  "PWMAPT"
#define ZJ_LOCAL_LOG_LVL LOG_LVL_INFO

#define ZJ_LOG(fmt, ...)  LOG(ZJ_LOCAL_LOG_LVL, "["TAG"]"fmt, ##__VA_ARGS__)

void zj_pwm_red_channel_init(int freq)
{
    ZJ_LOG("[%s:%d]freq:%dhz\r\n", __func__, __LINE__, freq);

    pwm_init(freq, 0, PORT_CONFIG_PWM_RED_CHANNEL, GPIO_A, GPIO_PIN_0);
    pwm_start(PORT_CONFIG_PWM_RED_CHANNEL);
}

void zj_pwm_green_channel_init(int freq)
{
    pwm_init(freq, 0, PORT_CONFIG_PWM_GREEN_CHANNEL, GPIO_A, GPIO_PIN_0);
    pwm_start(PORT_CONFIG_PWM_RED_CHANNEL);
}

void zj_pwm_blue_channel_init(int freq)
{
    ZJ_LOG("[%s:%d]freq:%dhz\r\n", __func__, __LINE__, freq);
    pwm_init(freq, 0, PORT_CONFIG_PWM_BLUE_CHANNEL, GPIO_A, GPIO_PIN_0);
    pwm_start(PORT_CONFIG_PWM_RED_CHANNEL);
}

void zj_pwm_warm_channel_init(int freq)
{
    ZJ_LOG("[%s:%d]freq:%dhz\r\n", __func__, __LINE__, freq);
    pwm_init(freq, 0, PORT_CONFIG_PWM_WARM_CHANNEL, GPIO_A, GPIO_PIN_0);
    pwm_start(PORT_CONFIG_PWM_RED_CHANNEL);
}

void zj_pwm_cold_channel_init(int freq)
{
    ZJ_LOG("[%s:%d]freq:%dhz\r\n", __func__, __LINE__, freq);
    pwm_init(freq, 0, PORT_CONFIG_PWM_COLD_CHANNEL, GPIO_A, GPIO_PIN_0);
    pwm_start(PORT_CONFIG_PWM_RED_CHANNEL);
}

void zj_pwm_init(int freq)
{

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
    float ln_duty = (float)(duty *100.0f/25500);
    ZJ_LOG("[%s:%d] ch:%d, duty:%d-%.2f\r\n", __func__, __LINE__, ch, duty, ln_duty);
    pwm_set_duty(ln_duty, ch);
}
