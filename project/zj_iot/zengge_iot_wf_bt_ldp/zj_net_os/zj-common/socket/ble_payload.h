#ifndef __PAYLOAD_H__
#define __PAYLOAD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "endian.h"
#include <string.h>

#define PRO_CONFIG_TIMEOUT        (10000)
#define PRO_CONFIG_BUF_SIZE       (2100)

#define PROTOCOL_TYPE_CMD      (0)
#define PROTOCOL_TYPE_DATA     (1)

#define    PRO_OK               0
#define    PRO_NOT_READY        1
#define    PRO_ERROR            2
#define    PRO_NO_MEM           3
#define    PRO_TIMEOUT          4


void pro_payload_is_head(uint8_t flag);
int payload_read (uint8_t *dst_buf, uint8_t *dst_type,
                             int *dst_len, uint8_t *src_buf, int src_len);
int payload_write (uint8_t *dst_buf,
                              int *dst_len, uint8_t src_id,
                              uint8_t *src_buf, int src_len,
                              int16_t remain_len);

#ifdef __cplusplus
}
#endif

#endif
