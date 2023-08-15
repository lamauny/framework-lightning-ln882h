#ifndef ZG_TCPLIST_UTILS_H_
#define ZG_TCPLIST_UTILS_H_

#ifdef USE_TCP_SERVER_RAW

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "lwip/altcp_tcp.h"

typedef struct {
   struct altcp_pcb *pcb;
   uint16_t timeout_ticks;
}tcpc_list_t;


 typedef struct __TCPCLS_NODE//节点声明
  {
      tcpc_list_t tcp_clist;
      struct __TCPCLS_NODE* next;
  } TCPCLS_NODE, *TCPCLS_PNODE;
  
TCPCLS_PNODE utils_tcpc_ls_list_updateNode(TCPCLS_PNODE *pHead, tcpc_list_t *tcps_inf);
bool utils_tcpc_ls_list_DeleteNode(TCPCLS_PNODE *pHead, tcpc_list_t *tcps_inf);
void utils_tcpc_ls_list_desrtroy(TCPCLS_PNODE *pHead);
int utils_tcpc_ls_list_get_node_num(TCPCLS_PNODE head);

#endif

#endif 
