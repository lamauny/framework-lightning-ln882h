#ifndef ZJ_OTA_API_H
#define ZJ_OTA_API_H

#define OTA_OPCODE_WRITE_START   0x01
#define OTA_OPCODE_WRITE_PAYLOAD 0x02
#define OTA_OPCODE_WRITE_END     0x03

#define RESP_OK               0
#define RESP_ERROR_ADDR_ERROR 1
#define RESP_ERROR_SEQ        2
#define RESP_ERROR_DEV_INFO   3
#define RESP_ERROR_CRC        4

void zj_ota_start(uint8_t *data,int len);
void ota_data_dec(uint8_t key,uint8_t *inoutdata,int len);
uint32_t do_crc(uint32_t crc,uint8_t *ptr,int len);

#endif