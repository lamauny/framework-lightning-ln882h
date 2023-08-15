#ifndef __SM2235Ex_H
#define __SM2235Ex_H



#include "version_config.h"

#ifdef APPCONFIG_OUTPUT_DRV_IIC

#if APPCONFIG_IC_TYPE == IIC_CHIP_EGH
// SM2235EHG
typedef enum{
	RGB_4mA = 0,
	RGB_8mA,
	RGB_12mA,
	RGB_16mA,
	RGB_20mA,
	RGB_24mA,
	RGB_28mA,
	RGB_32mA,
	RGB_36mA,
	RGB_40mA,
	RGB_44mA,
	RGB_48mA,
	RGB_52mA,
	RGB_60mA,
	RGB_64mA,
	RGB_MAX_ELECTRICITY = RGB_64mA
}SM_RGB_ELECTRICITY_E;

typedef enum{
	WY_5mA = 0,
	WY_10mA,
	WY_15mA,
	WY_20mA,
	WY_25mA,
	WY_30mA,
	WY_35mA,
	WY_40mA,
	WY_45mA,
	WY_50mA,
	WY_55mA,
	WY_60mA,
	WY_65mA,
	WY_70mA,
	WY_75mA,
	WY_80mA,
	WY_MAX_ELECTRICITY = WY_80mA
}SM_WY_ELECTRICITY_E;
#endif 



void SM2235Ex_Init(void);
int	 SM2235Ex_setElectricity_number(uint8_t rgb_ele,uint8_t wy_ele);
int  SM2235Ex_setGray(uint16_t R_LEVE, uint16_t G_LEVE,uint16_t B_LEVE,uint16_t W_LEVE,uint16_t Y_LEVE);

#endif

#endif





