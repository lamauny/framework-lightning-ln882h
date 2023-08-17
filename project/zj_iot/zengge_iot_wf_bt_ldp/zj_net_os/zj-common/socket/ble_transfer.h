/** @file
 *  @brief protocol
 */
#ifndef __TRANS_LAYER_H__
#define __TRANS_LAYER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "ZG_system.h"

#define BLE_PROV_BUF_SIZE 256
#define PRO_CONFIG_TIMEOUT        (10000)
#define PRO_CONFIG_BUF_SIZE       (2100)

#define PRO_CTRL_RETRY_BIT         (7)
#define PRO_CTRL_FRAG_BIT          (6)
#define PRO_CTRL_PORTE_BIT         (5)
#define PRO_CTRL_ACK_BIT           (4)
#define PRO_CTRL_TYPE_BIT          (2)
#define PRO_CTRL_VERSION_BIT       (0)

#define PACK_FRAG_END_BIT  (15)

#define PRO_RETRY         (1)
#define PRO_NOT_RETRY     (0)

#define PRO_VERSION00     (0)
#define PRO_VERSION01     (1)

#define PRO_TYPE_CMD      (0)
#define PRO_TYPE_DATA     (1)

#define PRO_ACK_PACK      (1)
#define PRO_NOT_ACK_PACK  (0)

#define PRO_PORTECTE      (1)
#define PRO_NOT_PORTECTE  (0)

 typedef void (*ble_recv_cb_t)(uint8_t cmdid, uint8_t *data,int len,bool is_ble);

struct pro_event {

    int event_id;
    void *p_buf;
    uint32_t length;
};


struct pro_dev {

    uint8_t *pyld_buf;
    struct pro_event ev;
    uint16_t tol_len_now;
    uint16_t total_length;
    uint8_t cmd_id;
    uint16_t frag_num;
};

struct general_head {
    uint8_t ctrl;
    uint8_t seq;
    uint16_t frag_ctrl;
};

typedef struct 
{
    int len;
    uint8_t buff[PRO_CONFIG_BUF_SIZE];
    uint8_t cmd_id;
}ack_buff_t;

typedef struct  {
    uint8_t buf[BLE_PROV_BUF_SIZE];
    uint32_t len;
}queue_buf;

int ble_pro_read (uint8_t *buf,size_t bytes);
void ble_pro_trans_deinit();
void ble_pro_trans_init (ble_recv_cb_t cb );
int ble_pro_trans_parse ( const void *buf,
                     size_t bytes);
int ble_pro_send (uint8_t seq,uint8_t ack, uint8_t type,const void *p_data, uint16_t length);
void ble_cmdid_event_post(uint8_t cmidid,uint8_t *data,int len,bool is_ble);                 

#ifdef __cplusplus
}
#endif

#endif

