#ifndef _ENCRYPTION_H_
#define _ENCRYPTION_H_


#ifdef __cplusplus
extern "C" {
#endif
	
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define ZG_CODE_KEY                     "1112131415161718"
#define KEY_READ_SIZE    512

int aes_key_init_1M();
int aes_key_init();
int aes_encrypt_data(const uint8_t *key,uint8_t *src,uint8_t *dest);
int aes_decrypt_data(const uint8_t *key,uint8_t *src,uint8_t *dest,int len);
char *aes_get_mac_ciphertext();
uint8_t aes_get_secret_key_version();
bool aes_key_self_test();
void aes_key_write_to_flash(uint8_t *new_key,int len);
bool is_one_machine_one_secret();
char *aes_get_key_flash_data();

#ifdef __cplusplus
}
#endif



#endif 