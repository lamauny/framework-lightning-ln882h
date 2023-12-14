
#include "zj_adapt_api.h"
#include "zj_adapt_config.h"
#include "runtime/runtime.h"
#include "debug/log.h"

static void ln_delay_us(uint32_t us)
{
    while(us--){
        for(int i = 0; i < 30; i ++)
            __NOP();

    }
}

void zj_timer_delay_us(uint32_t us)
{
    // LOG(LOG_LVL_INFO, "=>us:%d\r\n", us);
    // ln_block_delayus(us);
    // LOG(LOG_LVL_INFO, "us end\r\n");
    ln_delay_us(us);
}
