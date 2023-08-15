#ifndef ZG_SCENE_LIST_H_
#define ZG_SCENE_LIST_H_


#define  RMT_SCENE_MAX_STORE_SIZE      4096
#define  RMT_SCENE_MAX_STORE_NUM       64
#define  RMT_SCENE_PER_STORE_SECTION   (RMT_SCENE_MAX_STORE_SIZE / RMT_SCENE_MAX_STORE_NUM)
#define  RMT_SCENE_PER_STORE_PSIZE     RMT_SCENE_PER_STORE_SECTION - 16 

typedef struct 
{
	uint32_t tick;
	uint16_t rs_count;
    uint8_t  scene_id;
	uint8_t  btr_addr[4];
	uint8_t  param[RMT_SCENE_PER_STORE_PSIZE];
}scene_msg_t;

typedef struct __SCENE_NODE//节点声明
{
	   
	scene_msg_t msg;
	struct __SCENE_NODE* next;
} SCENE_NODE, *SCENE_PNODE;

int scene_list_get_num(SCENE_PNODE head);
SCENE_PNODE scene_list_join(SCENE_PNODE *pHead, scene_msg_t *msg);
int scene_list_delete(SCENE_PNODE *pHead, uint8_t scene_id,uint8_t *addr);


#endif 
