#ifndef ZG_IR_H
#define ZG_IR_H

#define GPIO_EINT_PORT			
#define GPIO_EINT_PIN		


#define IR_POTOCOL_NEC          0
#define IR_POTOCOL_RC5          1  //not support yet 

#define IR_NEC_STATE_IDLE       0
#define IR_NEC_STATE_PRE        1
#define IR_NEC_STATE_CMD        2
#define IR_NEC_STATE_REPEAT     3

#define IR_NEC_BIT_NUM 		    8
#define IR_NEC_MAGIN_US 		80
#define IR_NEC_TM_PRE_US 		1350
#define IR_NEC_D1_TM_US 		225
#define IR_NEC_D0_TM_US 		112
#define IR_NEC_REP_TM1_US 	    2000
#define IR_NEC_REP_TM2_US 	    1125
#define IR_NEC_REP_LOW_US 	    225
#define IR_NEC_REP_CYCLE 	    11000

#define IR_NEC_HEADER_HIGH_US   9000
#define IR_NEC_HEADER_LOW_US 	4500
#define IR_NEC_DATA_HIGH_US 	560
#define IR_NEC_DATA_LOW_1_US 	1690
#define IR_NEC_DATA_LOW_0_US 	560


void zj_ir_init();
int ir_get_key(unsigned char  *key,int time);
unsigned char ir_get_repeat();
unsigned int  ir_get_address();

#endif