#ifndef ZJ_QUICKLZ_H_
#define ZJ_QUICKLZ_H

int zj_quicklz_compress(uint8_t *buffer,size_t file_size,uint8_t *cmprs_buffer);
int zj_quicklz_get_decompress_block(uint8_t *buffer_hdr);
int zj_quicklz_decompress(uint8_t* buffer,size_t file_size,uint8_t *dcmprs_buffer);

#endif