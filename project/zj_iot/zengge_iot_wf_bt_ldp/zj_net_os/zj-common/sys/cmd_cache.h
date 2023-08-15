#ifndef __CMD_CACHE_H__
#define __CMD_CACHE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum 
{
    CMD_SRC_APPC_BLE   = 0,
    CMD_SRC_APPC_LOCAL = 1,
    CMD_SRC_APPC_CLOUD = 2,
    CMD_SRC_CFG_AT     = 3,
    CMD_SRC_CFG_BLE    = 4,
    CMD_SRC_CLOCK      = 5,
    CMD_SRC_HARDWARE   = 6,
    CMD_SRC_REMOTE     = 7
}cmd_src_t;


typedef struct 
{
    uint32_t time_stamp;
    cmd_src_t src;
    uint8_t cmd_data[16];
}cmd_cache_t;

void cmd_cache_post(uint8_t *data,uint8_t len,cmd_src_t src);
cmd_cache_t *cmd_cache_read();

#ifdef __cplusplus
}
#endif


#endif /*#ifndef __TCP_PERF_H__*/

