#ifndef ZG_MMSGP_UTILS_H_
#define ZG_MMSGP_UTILS_H_

typedef enum{
   PWR_CONT_BY_LOCAL_TCP,
   PWR_CONT_BY_CLOUD_TCP,
   PWR_CONT_BY_BLE_GATT1,
   PWR_CONT_BY_BLE_GATT2,
   PWR_CONT_BY_CLOCK,
   PWR_CONT_BY_IR_RMT,
   PWR_CONT_BY_KEY,
   PWR_CONT_BY_CFG,
   PWR_CONT_BY_BLE_RMT,
   PWR_CONT_POWER_ON,
}pwr_reason_t;

typedef struct 
{
   uint16_t startup_num;
   uint32_t timestamp;
   uint8_t inf;
   uint8_t cfg_opcode[2];
}pwr_base_arg_t;

typedef struct 
{
   uint32_t sys_time;
   pwr_base_arg_t base;
}pwr_record_arg_t;

typedef struct __PWR_NODE//节点声明
{
		pwr_record_arg_t msg;
		struct __PWR_NODE* next;
} PWR_NODE, *PWR_PNODE;

void pwr_record_init();
void pwr_record_op_reason(uint8_t op1,uint8_t op2,pwr_reason_t reason);
void pwr_record(uint8_t psw);
void pwr_record_send(uint8_t channel);
void pwr_record_list_time_correction();
void pwr_record_pause();
void pwr_record_continue();

#endif 
