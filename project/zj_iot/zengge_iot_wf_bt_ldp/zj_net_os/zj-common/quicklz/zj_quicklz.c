/*
 * File : quicklz_test.c
 * this example is a very simple test program for the quicklz library,
 * using non-stream compress and decompress. If you want to use stream compress,
 * you need at least 100K of ROM for history buffer(not recommend), or you can custom  
 * header to storage the compress block size, and carry out stream compress by non-stream.
 *
 * COPYRIGHT (C) 2006 - 2018, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date          Author          Notes
 * 2018-02-05    chenyong     first version
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ZG_system.h"
#include "quicklz.h"

#define BLOCK_HEADER_SIZE              4

/* Buffer padding for destination buffer, least size + 400 bytes large because incompressible data may increase in size. */
#define BUFFER_PADDING                 QLZ_BUFFER_PADDING

#if QLZ_STREAMING_BUFFER != 0
    #error Define QLZ_STREAMING_BUFFER to a zero value for this demo
#endif

int zj_quicklz_compress(uint8_t *buffer,size_t file_size,uint8_t *cmprs_buffer)
{
    /* Start to compress file  */
    qlz_state_compress *state_compress = NULL;
   // uint8_t *cmprs_buffer = NULL;
    size_t cmprs_size = 0, block_size = 0, totle_cmprs_size = 0;
    int ret = 0;

    state_compress = (qlz_state_compress *) malloc(sizeof(qlz_state_compress));
    if (!state_compress)
    {
        zj_printf("[qlz] No memory for state_compress struct, need %d byte, or you can change QLZ_HASH_VALUES to 1024 !\n",
                sizeof(qlz_state_compress));
        ret = -1;
        goto _exit;
    }
    memset(state_compress, 0x00, sizeof(qlz_state_compress));
    zj_printf("[qlz]compress start : ");

    block_size = file_size;

    /* The destination buffer must be at least size + 400 bytes large because incompressible data may increase in size. */
    cmprs_size = qlz_compress(buffer, (char *) cmprs_buffer + BLOCK_HEADER_SIZE, block_size, state_compress);
    
    /* Store compress block size to the block header (4 byte). */
    cmprs_buffer[3] = cmprs_size % (1 << 8);
    cmprs_buffer[2] = (cmprs_size % (1 << 16)) / (1 << 8);
    cmprs_buffer[1] = (cmprs_size % (1 << 24)) / (1 << 16);
    cmprs_buffer[0] = cmprs_size / (1 << 24);

    totle_cmprs_size += cmprs_size + BLOCK_HEADER_SIZE;

    zj_printf("\n");
    zj_printf("[qlz]compressed %d bytes into %d bytes, compression ratio is %d%%!\n", file_size, totle_cmprs_size,
            (totle_cmprs_size * 100) / file_size);
_exit:

    if (state_compress)
    {
        free(state_compress);
    }
    
    return ret == 0 ? cmprs_size : ret;
}

int zj_quicklz_get_decompress_block(uint8_t *buffer_hdr)
{
    size_t block_size = buffer_hdr[0] * (1 << 24) + buffer_hdr[1] * (1 << 16) + buffer_hdr[2] * (1 << 8) + buffer_hdr[3];
    return block_size;
}

int zj_quicklz_decompress(uint8_t* buffer,size_t file_size,uint8_t *dcmprs_buffer)
{
   uint32_t g_take_time = ZG_system_time();
    /* Start to decompress file  */
    qlz_state_decompress *state_decompress = NULL;
    size_t dcmprs_size = 0;
    int ret = 0;

    if (file_size <= BLOCK_HEADER_SIZE)
    {
        zj_printf("[qlz] decomprssion file size : %d error!\n", file_size);
        ret = -1;
        goto _dcmprs_exit;
    }

    state_decompress = (qlz_state_decompress *) malloc(sizeof(qlz_state_decompress));
    if (!state_decompress)
    {
        zj_printf("[qlz] No memory for state_decompress struct!\n");
        ret = -1;
        goto _dcmprs_exit;
    }
    memset(state_decompress, 0x00, sizeof(qlz_state_decompress));

    zj_printf("[qlz]decompress start : ");
    dcmprs_size = qlz_decompress((const char *) buffer, dcmprs_buffer, state_decompress);

    zj_printf("\n");
    zj_printf("decompressed %d bytes into %d bytes %ldms!\n", file_size, dcmprs_size, (uint32_t)(ZG_system_time() - g_take_time));

_dcmprs_exit:

    if (state_decompress)
    {
        free(state_decompress);
    }

    return ret;
}

