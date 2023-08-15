#ifndef ZJ_BLE_OTA_ADAPT_H
#define ZJ_BLE_OTA_ADAPT_H


int zj_ota_boot_addr_read(uint32_t *OTA_flash_start_addr);
int zj_ota_boot_update(uint32_t newAddr);
void zj_ota_erase_area(uint32_t boot_start_addr);
int zj_ota_write(uint32_t addr, uint8_t *dat,int len);
int zj_ota_write_end();
int zj_ota_read(uint32_t addr,uint8_t *dat,int len);


#endif