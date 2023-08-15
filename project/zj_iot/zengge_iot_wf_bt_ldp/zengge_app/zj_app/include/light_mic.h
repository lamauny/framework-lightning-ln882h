#ifndef  LIGHT_MIC_H
#define  LIGHT_MIC_H

#include "version_config.h"

#ifdef APPCONFIG_SUPPORT_HW_MIC

void light_mic_init();
int light_mic_start();
int light_mic_stop();
void light_mic_set_sens(uint8_t sens);
uint8_t light_mic_get_sens();
void light_mic_set_color(uint8_t *pdata);
uint8_t *light_mic_get_color();

#endif

#endif