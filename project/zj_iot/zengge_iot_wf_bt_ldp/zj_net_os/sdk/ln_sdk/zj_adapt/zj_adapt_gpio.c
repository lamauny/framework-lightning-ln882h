
#include "zj_adapt_config.h"
#include "zj_adapt_api.h"
#include "hal/hal_gpio.h"
#include "hal/hal_common.h"
#include "ln882h.h"

#define    TAG  "GPIOAPT"
#define    ZJ_LOCAL_LOG_LVL LOG_LVL_INFO

#undef ZJ_LOG
#define ZJ_LOG(fmt, ...)  LOG(ZJ_LOCAL_LOG_LVL, "["TAG"] "fmt, ##__VA_ARGS__)

/**
 * GPIO mapping
 * [100 : 199] -> GPIOA  [100:115]
 * [200 : 255] -> GPIOB  [200:215]
*/

void zj_gpio_init(uint8_t io,zj_gpio_t status)
{
    gpio_init_t_def gpio_init;
    uint32_t gpio_base;

    ZJ_LOG("io:%d, status:%d\r\n", io, status);

    if (io < 100 || io > 215 || (io < 200 && io > 115)) {
        ZJ_LOG("io not in range <%d>!\r\n", io);
        return;
    }

    memset(&gpio_init, 0, sizeof(gpio_init));
    
    if (io >= 200) {
        gpio_init.pin = io - 200;
        gpio_base = GPIOB_BASE;
    } else {
        gpio_init.pin = io - 100;
        gpio_base = GPIOA_BASE;
    }

    gpio_init.speed = GPIO_HIGH_SPEED;
    gpio_init.mode = GPIO_MODE_DIGITAL;

    if(status == ZJ_GPIO_INPUT) {
        gpio_init.pull = GPIO_PULL_UP;
        gpio_init.dir = GPIO_INPUT;
    } else {
        gpio_init.dir = GPIO_OUTPUT;
    }

    hal_gpio_init(gpio_base, &gpio_init);
}

int zj_gpio_intr_set_config(uint8_t io,zj_gpio_int_type_t type,void *serv)
{
    uint32_t gpio_base;
    gpio_pin_t pin;
    gpio_int_type_t intr_type;

    ZJ_LOG("io:%d, type:%d\r\n", io, type);

    if (io < 100 || io > 215 || (io < 200 && io > 115)) {
        ZJ_LOG("io not in range <%d>!\r\n", io);
        return -1;
    }

    if (io >= 200) {
        pin = io - 200;
        gpio_base = GPIOB_BASE;
    } else {
        pin = io - 100;
        gpio_base = GPIOA_BASE;
    }

    switch(type) {
        case ZJ_GPIO_INTR_POSEDGE:
            intr_type = GPIO_INT_RISING  ;
            break;
        case ZJ_GPIO_INTR_NEGEDGE:
            intr_type = GPIO_INT_FALLING ;
            break;
        case ZJ_GPIO_INTR_LOLEVEL:
            intr_type = GPIO_INT_FALLING;
            break;
        case ZJ_GPIO_INTR_HILEVEL:
            intr_type = GPIO_INT_RISING ;
            break;
        default:
            return -1;
    }
    hal_gpio_pin_it_cfg(gpio_base, pin, intr_type);

    return 0;
}

void zj_gpio_intr_disable(uint8_t io)
{
    uint32_t gpio_base;
    gpio_pin_t pin;

    ZJ_LOG("io:%d\r\n", io);

    if (io < 100 || io > 215 || (io < 200 && io > 115)) {
        ZJ_LOG("io not in range <%d>!\r\n", io);
        return;
    }

    if (io >= 200) {
        pin = io - 200;
        gpio_base = GPIOB_BASE;
    } else {
        pin = io - 100;
        gpio_base = GPIOA_BASE;
    }
    hal_gpio_pin_it_en(gpio_base, pin, HAL_DISABLE);
}

void zj_gpio_intr_enable(uint8_t io)
{
    uint32_t gpio_base;
    gpio_pin_t pin;

    ZJ_LOG("io:%d\r\n", io);

    if (io < 100 || io > 215 || (io < 200 && io > 115)) {
        ZJ_LOG("io not in range <%d>!\r\n", io);
        return;
    }

    if (io >= 200) {
        pin = io - 200;
        gpio_base = GPIOB_BASE;
    } else {
        pin = io - 100;
        gpio_base = GPIOA_BASE;
    }

    hal_gpio_pin_it_en(gpio_base, pin, HAL_ENABLE);
}

void zj_gpio_deinit(uint8_t io)
{
    uint32_t gpio_base;

    ZJ_LOG("io:%d\r\n", io);

    if (io < 100 || io > 215 || (io < 200 && io > 115)) {
        ZJ_LOG("io not in range <%d>!\r\n", io);
        return;
    }

    if (io >= 200) {
        gpio_base = GPIOB_BASE;
    } else {
        gpio_base = GPIOA_BASE;
    }
    hal_gpio_deinit(gpio_base);
}

void zj_gpio_output_set(uint8_t io,uint8_t status)
{
    uint32_t gpio_base;
    gpio_pin_t pin;

    ZJ_LOG("io:%d, status:%d\r\n", io, status);

    if (io < 100 || io > 215 || (io < 200 && io > 115)) {
        ZJ_LOG("io not in range <%d>!\r\n", io);
        return;
    }

    if (io >= 200) {
        pin = io - 200;
        gpio_base = GPIOB_BASE;
    } else {
        pin = io - 100;
        gpio_base = GPIOA_BASE;
    }

    if (status == 0) {
        hal_gpio_pin_reset(gpio_base, pin);
    } else {
        hal_gpio_pin_set(gpio_base, pin);
    }
}

uint8_t zj_gpio_input_get(uint8_t io)
{
    uint32_t gpio_base;
    gpio_pin_t pin;

    ZJ_LOG("io:%d, status:%d\r\n", io);

    if (io < 100 || io > 215 || (io < 200 && io > 115)) {
        ZJ_LOG("io not in range <%d>!\r\n", io);
        return;
    }

    if (io >= 200) {
        pin = io - 200;
        gpio_base = GPIOB_BASE;
    } else {
        pin = io - 100;
        gpio_base = GPIOA_BASE;
    }

    return hal_gpio_pin_input_read(gpio_base, pin);
}
