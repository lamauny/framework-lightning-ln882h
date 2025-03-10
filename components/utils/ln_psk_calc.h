
#ifndef __LN_PSK_CALC_H__
#define __LN_PSK_CALC_H__

#include <stdint.h>

#define LN_SW_PSK_USING_FASTPBKDF2_ALGORITHM (0)

int ln_psk_calc(const char *ssid, const char *pwd, uint8_t *psk_buff, uint8_t psk_buff_size);

#endif // !__LN_PSK_CALC_H__
