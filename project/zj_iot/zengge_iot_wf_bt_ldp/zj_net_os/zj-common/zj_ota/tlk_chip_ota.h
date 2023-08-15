#ifndef ZJ_TLK_OTA_API_H
#define ZJ_TLK_OTA_API_H

int tlk_ota_init();
int tlk_ota_handler();
void tlk_ota_resp_code_set(uint8_t code);
bool is_tlk_ota_running();

#endif