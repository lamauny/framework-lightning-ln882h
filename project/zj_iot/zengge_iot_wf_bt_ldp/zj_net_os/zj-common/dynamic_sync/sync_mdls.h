
#ifndef ZG_MDLS_H_
#define ZG_MDLS_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define ZMS_DBG     OS_LOGI  

#ifndef CONFIG_SYNC_HB_SIZE
#define CONFIG_SYNC_HB_SIZE  8
#endif

typedef struct 
{   
    uint32_t addr;
    uint8_t heartbeat[CONFIG_SYNC_HB_SIZE];
    bool online;
    uint32_t sys_tick;
}node_hb_t;


 typedef struct __MDLS_HB_NODE//节点声明
  {
      node_hb_t hb_inf;
      struct __MDLS_HB_NODE* next;
  } MDLS_HB_NODE, *MDLS_HB_PNODE;
  
void hb_list_desrtroy(MDLS_HB_PNODE *pHead);
int hb_list_get_node_num(MDLS_HB_PNODE head);
int hb_list_update_node(MDLS_HB_PNODE *pHead, node_hb_t *sinf);  
MDLS_HB_PNODE hb_list_FindNodeByAddr(MDLS_HB_PNODE head, uint32_t addr);
void hb_list_DeleteNode(MDLS_HB_PNODE *pHead, uint32_t addr);

#endif 