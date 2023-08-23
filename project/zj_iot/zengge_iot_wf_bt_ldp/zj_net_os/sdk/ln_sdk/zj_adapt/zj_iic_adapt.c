
#include "zj_adapt_api.h"
#include "zj_adapt_config.h"
#include "runtime/runtime.h"

void zj_timer_delay_us(uint32_t us)
{
    ln_block_delayus(us);
}
