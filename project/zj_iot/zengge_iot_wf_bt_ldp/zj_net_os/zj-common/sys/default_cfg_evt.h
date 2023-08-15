
#ifndef __DEF_CFG_H__
#define __DEF_CFG_H__


typedef void (*def_cfg_event_cb_t)(uint8_t opcode, uint16_t checksum ,uint8_t *payload);

void def_cfg_post(uint8_t opcode, uint16_t checksum ,uint8_t *payload);
void register_def_cfg_event(def_cfg_event_cb_t cb);


#endif 

