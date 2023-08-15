#ifndef __BP5758D_H
#define __BP5758D_H


#include "version_config.h"

#ifdef APPCONFIG_OUTPUT_DRV_IIC

void BP5758D_Init(void);
int  BP5758D_setElectricity_number(uint8_t r_ele,uint8_t g_ele,uint8_t b_ele,uint8_t w_ele,uint8_t c_ele);
int  BP5758D_setGray(uint16_t R_LEVE, uint16_t G_LEVE,uint16_t B_LEVE,uint16_t W_LEVE,uint16_t Y_LEVE);

#endif

#endif





