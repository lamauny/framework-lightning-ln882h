#ifndef SYNC_HBM_H
#define SYNC_HBM_H

#include "sync_mdls.h"

typedef struct  {
    void (*sync_enable_condition_get) (void(*data_get)(void *));
    void (*heartbeat_payload_send_get) (void(*data_get)(void *));
    void (*sync_election_results) (uint8_t is_team_leader);
}dynamic_func_t;

void dynamic_sync_team_leader_election();
uint32_t dynamic_sync_get_leader_address();
void dynamic_sync_hbm();
void dynamic_sync_update_node(node_hb_t *hb_inf);
void dynamic_sync_cmd_send(uint8_t *data,uint16_t len);
void register_dynamic_sync_callback(dynamic_func_t cb);


#endif
