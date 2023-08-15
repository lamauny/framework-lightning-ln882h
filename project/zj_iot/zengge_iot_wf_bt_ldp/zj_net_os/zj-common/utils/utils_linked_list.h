#ifndef APP_LIST_H_
#define APP_LIST_H_


#ifdef __cplusplus
extern "C" {

#endif
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define LIST_LENGTH      4

 typedef struct _NODE//节点声明
  {
      char btr_addr[LIST_LENGTH];
      struct _NODE* next;
  } NODE, *PNODE;
  
void print_list(PNODE head);
void list_insertHead(PNODE *pHead, char* val);
void list_insertTail(PNODE *pHead, char* val);
void list_deleteHead(PNODE *pHead);
void list_deleteTail(PNODE *pHead);
PNODE list_findByAllVal(PNODE head, uint8_t *val,int len);
PNODE list_findByIndex(PNODE head, uint8_t index);
void list_insertByIndex(PNODE *pHead, uint8_t index, char* val);
void list_deleteByIndex(PNODE *pHead, uint8_t index);
void list_deleteByAllVal(PNODE *pHead, uint8_t *val);
void list_desrtroy(PNODE *pHead);
int list_get_node_number(PNODE head);
void list_deleteAllbyVal(PNODE *pHead, uint8_t *val,uint8_t len);
#ifdef __cplusplus
}
#endif



#endif 