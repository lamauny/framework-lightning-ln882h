#ifndef ZJ_SIG_OTA_API_H
#define ZJ_SIG_OTA_API_H

bool is_uart_ota_running();
void uart_ota_init();
void uart_ota_resp_handle(uint8_t *data,int len);
int uart_ota_handler();

#endif