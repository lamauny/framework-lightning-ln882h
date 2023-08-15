
#ifndef BLE_CENTRAL_EVT__H
#define BLE_CENTRAL_EVT__H

typedef enum 
{
    BLE_CENT_SCAN_RESULT_EVT,
    BLE_CENT_NOTIFY_SRC_DATA,
}ble_central_evt_t;

typedef struct 
{
    uint16_t product_uuid;
    uint16_t ota_type;
    uint8_t  typeID;
    uint16_t fw_version;
    uint8_t pro_version;
}tlk_sub_base_inf;


typedef struct 
{
    ble_central_evt_t evt;
    int rssi;
    uint8_t scan_type;
    uint8_t scan_mac[6];
    tlk_sub_base_inf base_inf;
    uint8_t *payload;
    uint8_t p_len;
}ble_central_arg_t;

typedef void (*ble_central_cb_t)(ble_central_arg_t *arg);

int ble_central_evt_send(ble_central_arg_t *arg);
int ble_central_evt_read(uint32_t time,ble_central_arg_t *evt);

void ble_central_scan_result_post(ble_central_arg_t *evt);
void ble_central_regist_scan_evt_cb(ble_central_cb_t cb);


#endif