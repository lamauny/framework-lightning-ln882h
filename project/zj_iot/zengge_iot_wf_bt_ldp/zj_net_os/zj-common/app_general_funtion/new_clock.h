#ifndef _NEW_LIGHT_CLOCK_H
#define _NEW_LIGHT_CLOCK_H

bool what_day_of_week(unsigned char curren_week,unsigned char week_cfg);
void new_clock_loop_handler(struct rtc_time Real_time);
void new_clock_init();
void new_clock_clear();
void new_clock_set_config(unsigned char number,unsigned char *single_clock_data,int len);
int new_clock_get_data_len();
void new_clock_get_config(unsigned char *data);

#endif