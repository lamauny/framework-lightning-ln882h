#ifndef __SM2135Ex_H
#define __SM2135Ex_H


#include "version_config.h"

#ifdef APPCONFIG_OUTPUT_DRV_IIC

#if APPCONFIG_IC_TYPE == IIC_CHIP_EH
// SM2135EH_7W
typedef enum{
	RGB_9mA = 0,
	RGB_14mA,
	RGB_19mA,
	RGB_24mA,
	RGB_29mA,
	RGB_34mA,
	RGB_39mA,
	RGB_44mA,
	RGB_MAX_ELECTRICITY = RGB_44mA
}SM_RGB_ELECTRICITY_E;

typedef enum{
	WY_0mA = 0,
	WY_5mA,
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
	WY_59mA,
	WY_63mA,
	WY_67mA,
	WY_72mA,
	WY_MAX_ELECTRICITY = WY_72mA
}SM_WY_ELECTRICITY_E;
#endif 

#if APPCONFIG_IC_TYPE == IIC_CHIP_EJ
//SM2135EJ_9W
typedef enum{
	RGB_14_3mA = 0,
	RGB_21_4mA,
	RGB_28_5mA,
	RGB_35_6mA,
	RGB_42_7mA,
	RGB_49_8mA,
	RGB_56_9mA,
	RGB_64_0mA,
	RGB_MAX_ELECTRICITY = RGB_64_0mA
}SM_RGB_ELECTRICITY_E;

typedef enum{
	WY_0mA = 0,
	WY_7_3mA,
	WY_14_4mA,
	WY_21_5mA,
	WY_28_6mA,
	WY_35_7mA,
	WY_42_8mA,
	WY_50_0mA,
	WY_56_7mA,
	WY_63_5mA,
	WY_70_2mA,
	WY_77_0mA,
	WY_83_8mA,
	WY_90_0mA,
	WY_MAX_ELECTRICITY = WY_90_0mA
}SM_WY_ELECTRICITY_E;
#endif 

void SM2135Ex_Init(void);
int  SM2135Ex_setGray(uint8_t R_LEVE, uint8_t G_LEVE,uint8_t B_LEVE,uint8_t W_LEVE,uint8_t Y_LEVE);
int  SM2135EH_setElectricity_number(uint8_t rgb_ele,uint8_t wy_ele);
int  SM2135EJ_setElectricity_number(uint8_t rgb_ele,uint8_t wy_ele);

#endif
#endif





