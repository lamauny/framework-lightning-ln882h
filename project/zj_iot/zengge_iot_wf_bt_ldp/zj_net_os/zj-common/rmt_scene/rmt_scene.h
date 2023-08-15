#ifndef ZG_SCENE_CFG_H_
#define ZG_SCENE_CFG_H_

#define SCENE_OK       0
#define SCENE_ERR_SIZE -1

int rmt_scene_clear(uint8_t btr_addr[4]);
int rmt_scene_save(uint8_t btr_addr[4],uint8_t scene_id,uint8_t *payload,uint8_t size);
int rmt_scene_load(uint8_t btr_addr[4],uint8_t scene_id,uint8_t *payload,uint8_t size);
int rmt_scene_copy(uint8_t src_btr_addr[4],uint8_t target_btr_addr[4]);

#endif 
