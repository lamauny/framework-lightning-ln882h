
#include "zj_adapt_config.h"
#include "zj_adapt_api.h"
#include "hal/hal_gpio.h"

#define    TAG  "GPIOAPT"
#define    ZJ_LOCAL_LOG_LVL LOG_LVL_INFO

#undef ZJ_LOG
#define ZJ_LOG(fmt, ...)  LOG(ZJ_LOCAL_LOG_LVL, "["TAG"] "fmt, ##__VA_ARGS__)

void zj_gpio_init(uint8_t io,zj_gpio_t status)
{
    gpio_init_t_def gpio_init;

    ZJ_LOG("io:%d, status:%d\r\n", io, status);
    // hal_gpio_init(GPIOA_BASE, &gpio_init);
	//zero-initialize the config structure.
	// gpio_config_t io_conf = {};
	// //disable interrupt
	// io_conf.intr_type = GPIO_INTR_DISABLE;
	// //set as output mode
	if(status == ZJ_GPIO_INPUT){

	// 	io_conf.mode = GPIO_MODE_INPUT;
	// 	//enable pull-up mode
	// 	io_conf.pull_up_en = 1;
	}else{
	// 	io_conf.mode = GPIO_MODE_OUTPUT;
	// 	//enable pull-down mode
	// 	io_conf.pull_down_en = 1;
	}
	// //bit mask of the pins that you want to set,e.g.GPIO18/19
	// io_conf.pin_bit_mask = (1ULL<<io);

	// //configure GPIO with the given settings
	// gpio_config(&io_conf);
}

int zj_gpio_intr_set_config(uint8_t io,zj_gpio_int_type_t type,void *serv)
{

   return 0;
}

void zj_gpio_intr_disable(uint8_t io)
{

}

void zj_gpio_intr_enable(uint8_t io)
{

}

void zj_gpio_deinit(uint8_t io)
{

}

void zj_gpio_output_set(uint8_t io,uint8_t status)
{
	// gpio_set_level(io,status);
}

uint8_t zj_gpio_input_get(uint8_t io)
{
	// return gpio_get_level(io);
	return 0;
}
