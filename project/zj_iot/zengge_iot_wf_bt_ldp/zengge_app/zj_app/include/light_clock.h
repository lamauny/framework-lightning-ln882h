#ifndef __CLOCK_H__
#define __CLOCK_H__


#ifdef __cplusplus
extern "C" {
#endif
  
  
void  light_old_clock_init();
void old_clock_set_config(unsigned char *dat,int len);
unsigned char * old_clock_get_config();
void timer_clock_start();
void old_clock_clear();

#ifdef __cplusplus
}
#endif


#endif 
