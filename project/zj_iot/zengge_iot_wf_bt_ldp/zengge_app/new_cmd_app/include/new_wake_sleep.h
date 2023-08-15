#ifndef _NEW_WAKE_SLEEP__H
#define _NEW_WAKE_SLEEP__H

#include "ZG_system.h"

void wake_up_loop_handler(struct rtc_time Real_time);
void fall_asleep_loop_handler(struct rtc_time Real_time);

void wake_up_set_config(uint8_t *data);
void fall_asleep_set_config(uint8_t *data);

#endif