#include "stdint.h"
#include "stdlib.h"
#include "hal/hal_flash.h"

static uint8_t g_chip_uid[16]      = {0};
static uint8_t g_chip_wifi_mac[6]  = {0x00, 0x50, 0x00, 0x00, 0x00, 0x00};


static uint32_t djb_hash_hexdata(const char *input, uint32_t len)
{
    uint32_t hash = 5381;
    int c = *input;

    for (uint32_t i = 0; i < len; i++)
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        input++;
        c = *input;
    }
    return hash;
}

void ln_hash_mac_gen(uint8_t *mac)
{
    uint8_t chip_uid[16]      = {0};
    uint32_t hash = 0;
    hal_flash_read_unique_id(chip_uid);
    hash = djb_hash_hexdata((const char *)chip_uid, sizeof(chip_uid));
    mac[0] = 0x00;
    mac[1] = 0x50;
    mac[2] = (uint8_t)((hash) & 0xFF);
    mac[3] = (uint8_t)((hash >> 8) & 0xFF);
    mac[4] = (uint8_t)((hash >> 16) & 0xFF);
    mac[5] = (uint8_t)((hash >> 24) & 0xFF);
}
