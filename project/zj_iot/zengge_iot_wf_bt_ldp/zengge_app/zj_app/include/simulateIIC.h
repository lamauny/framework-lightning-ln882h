#ifndef __SIMULATE_IIC_H
#define __SIMULATE_IIC_H


#include "version_config.h"
#ifdef APPCONFIG_OUTPUT_DRV_IIC


void simulateIIC_Init(void);
void simulateIIC_DeInit(void);

void simulateIIC_Send_ByteBuf(uint8_t *dataBuf, uint8_t length);
void simulateIIC_enterStandb(void);


#endif


#endif




