#ifndef ZJ_SYS_ADAPT_H
#define ZJ_SYS_ADAPT_H

#include <stdint.h>
#include "lwip/ip_addr.h"

#ifdef CONFIG_OLD_V5_ADV
#define BLE_NET_VERSION 5
#define BLE_ADV_DATA_LENGTH   25
#endif

#ifndef BLE_NET_VERSION
#define BLE_NET_VERSION       0x07
#define BLE_ADV_DATA_LENGTH   31
#endif

struct rtc_time {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
};

typedef enum {
    ZJ_GPIO_INTR_DISABLE = 0,
    ZJ_GPIO_INTR_POSEDGE = 1,
    ZJ_GPIO_INTR_NEGEDGE = 2,
    ZJ_GPIO_INTR_ANYEDGE = 3,
    ZJ_GPIO_INTR_LOLEVEL = 4,
    ZJ_GPIO_INTR_HILEVEL = 5
} zj_gpio_int_type_t;

typedef enum
{
	ZJ_GPIO_OUTPUT,
	ZJ_GPIO_INPUT,
	ZJ_GPIO_INTR
}zj_gpio_t;

typedef struct{

	uint8_t *data;
	int len;
}uart_data_t;

#if defined(USE_1M_PARTITION) || defined(USE_2M_PARTITION)

#else
enum WIFI_STATE_ENUM_LIST {
    WIFI_STATE_UNKNOWN                              = 0x00,
    WIFI_STATE_IDLE                                 = 0x01,
    WIFI_STATE_CONNECTING                           = 0x02,
    WIFI_STATE_CONNECTED_IP_GETTING                 = 0x03,
    WIFI_STATE_CONNECTED_IP_GOT                     = 0x04,
    WIFI_STATE_DISCONNECT                           = 0x05,
    WIFI_STATE_WITH_AP_IDLE                         = 0x11,
    WIFI_STATE_WITH_AP_CONNECTING                   = 0x12,
    WIFI_STATE_WITH_AP_CONNECTED_IP_GETTING         = 0x13,
    WIFI_STATE_WITH_AP_CONNECTED_IP_GOT             = 0x14,
    WIFI_STATE_WITH_AP_DISCONNECT                   = 0x15,
    WIFI_STATE_IFDOWN                               = 0x06,
    WIFI_STATE_SNIFFER                              = 0x07,
    WIFI_STATE_PSK_ERROR                            = 0x08,
    WIFI_STATE_NO_AP_FOUND                          = 0x09,
};
#endif

typedef enum 
{
	RESET_REASON_UNKNOWN,
	RESET_REASON_PWR_ON,   // Power on reset
	RESET_REASON_WDT,      // Super watch dog resets the chip
	RESET_REASON_BROWN_OUT,//VDD voltage is not stable and resets the chip
	RESET_REASON_SOFTWARE,
    RESET_REASON_HARFWARE,
	RESET_REASON_CRASH
}zj_reset_reason_t;
typedef enum{
    ADAPT_EVT_WIFI_INIT_DONE,
    ADAPT_EVT_BLE_INIT_DONE,
    ADAPT_EVT_WIFI_CONNECTED,
    ADAPT_EVT_WIFI_DISCONNECTED,
    ADAPT_EVT_BLE_CONNECTED,
    ADAPT_EVT_BLE_DISCONNECTED,
    ADAPT_EVT_BLE_LIGHT_DATA,
    ADAPT_EVT_BLE_OTA_DATA,
    ADAPT_EVT_AT_WIFI_SCAN,
    ADAPT_EVT_BLUFI_SCAN,
    ADAPT_EVT_PTEST_SCAN,
    ADAPT_EVT_OTA_STATUS,
    ADAPT_EVT_BLE_RMT,
    ADAPT_EVT_STORE,
    ADAPT_EVT_MESH_WIFI_SCAN
}zj_adapter_evt_t;

typedef struct  {
    void (*adapter_evt_cb_t) (zj_adapter_evt_t status,void *data1,void *data2,int len);
    void (*adapter_ble_advData_get) (void(*data_get)(void *));
    void (*adapter_ble_ScanResp_get) (void(*data_get)(void *));
    void (*adapter_ble_name_get) (void(*data_get)(void *));
    void (*adapter_ble_update_condition_get) (void(*data_get)(void *));
    void (*adapter_btr_relay_condition_get) (void(*data_get)(void *));
}zj_adapter_func_t;

void register_zj_adapter_callback(zj_adapter_func_t cb);
void zj_adapter_post_event(zj_adapter_evt_t evt,void *data1,void *data2,int len);
uint8_t *zj_adapter_get_adv_data_config();
uint8_t *zj_adapter_get_adv_scanResp_config();
uint8_t *zj_adapter_get_adv_name_config();
uint8_t zj_adapter_post_adv_update_condition_get();
uint8_t zj_adapter_post_btr_realy_condition_get();

void zj_adapter_init();

typedef struct 
{
    uint8_t bssid[6];                     /**< MAC address of AP */
    uint8_t ssid[33];                     /**< SSID of AP */
    uint8_t channel;                      /**< channel of AP */
    int8_t  rssi;                         /**< signal strength of AP */
    uint8_t authmode;                     /**< authmode of AP */
}zj_wifi_scan_inf_t;


/*****************************flash api适配****************************************/
void zj_flash_write(uint32_t addr , uint8_t *pbuf,int len);
void zj_flash_read(uint32_t addr , uint8_t *pbuf,int len);
void zj_flash_erase(uint32_t addr,uint32_t size);
void zj_userdata_idle_write_init();
uint8_t zj_userdata_check_writeStatus();
void zj_userdata_disable();
void zj_userdata_enable();
void zj_userdata_delete_key(char *key);
int zj_userdata_write(char *key, uint8_t *dat,int len);
int zj_userdata_read(char *key, uint8_t *dat,int len);
void zj_restore_userdata();
/*****************************pwm api适配****************************************/
void zj_pwm_init();
void zj_pwm_red_channel_init(int freq);
void zj_pwm_green_channel_init(int freq);
void zj_pwm_blue_channel_init(int freq);
void zj_pwm_warm_channel_init(int freq);
void zj_pwm_cold_channel_init(int freq);

void zj_pwm_set_duty(uint8_t ch,int duty);
/*****************************system api适配****************************************/
void zj_watchdog_start();
int zj_watchdog_stop(uint32_t time);

int zj_ota_get_image(char *http_url);
int zj_ota_verify_image();
void zj_ota_reboot();

void zj_restart_set_time(uint32_t time);
int zj_restart_system(void);

void zj_rtc_set_time(struct rtc_time *time_set);
void zj_rtc_get_time(struct rtc_time *RTC_return);
uint32_t zj_get_current_timestamp();
uint32_t  mktimev(struct rtc_time *tm);
uint8_t is_zj_rtc_time_set();

uint32_t zj_get_freeheap();
/*****************************wifi api适配****************************************/

void zj_wifi_STA_Start(uint8_t *ssid,uint8_t ssid_len,uint8_t *pwd,uint8_t pwd_len);
void zj_wifi_STA_Stop();
void zj_wifi_AP_Start(uint8_t *ssid,uint8_t ssid_len,uint8_t *pwd,uint8_t pwd_len);
void zj_wifi_AP_Stop();
void zj_scan_router(zj_adapter_evt_t evt);
int wifi_mgmr_status_code_clean_internal();
void zj_wifi_drv_init();

void zj_wifi_set_ip_info(char *ip_addr);
void zj_wifi_get_ip_info(char *ip_addr);
void zj_wifi_get_gateway(char *ip_addr);
ip_addr_t zj_wifi_get_LAN_broadcast_addr();
ip_addr_t zj_wifi_get_ip_address();
int zj_wifi_get_mac_info(char *mac_str);
int zj_wifi_get_mac_hex(uint8_t *mac_hex);
int zj_wifi_get_state();
int zj_get_ap_rssi();
uint8_t zj_get_lan_code();
void zj_set_lan_code(uint8_t code);
/*****************************ble api适配****************************************/
void zj_ble_drv_init();
void zj_ble_scan_stop();
void zj_ble_scan_start();
void zj_btr_adv_payload_update(uint8_t *data);
void zj_ble_dev_status_adv_set(uint8_t *data_info,uint8_t len);
void zj_ble_adv_start();
void zj_ble_adv_stop();
void zj_ble_adv_update();
void zj_ble_notify(uint8_t *data,int len);
void zj_ble_ota_notify(uint8_t *data,int len);
uint8_t zj_ble_get_connected_status();
void ble_remote_queue_init();
int bt_remote_recv(uint8_t *arg ,int time);
uint16_t zj_ble_get_mtu();
uint8_t is_ble_init_finished();
/*****************************adc api适配****************************************/
void zj_adc_deinit();
void zj_adc_init();
uint8_t zj_adc_get_voltage();
void zj_adc_sens_cfg(uint8_t sens);

/*****************************gpio api适配****************************************/
void zj_gpio_deinit(uint8_t io);
void zj_gpio_init(uint8_t io,zj_gpio_t status);
void zj_gpio_output_set(uint8_t io,uint8_t status);
uint8_t zj_gpio_input_get(uint8_t io);
int zj_gpio_intr_set_config(uint8_t io,zj_gpio_int_type_t type,void *serv);
void zj_gpio_intr_disable(uint8_t io);
void zj_gpio_intr_enable(uint8_t io);

void zj_timer_delay_us(uint32_t us);

/*****************************串口api适配****************************************/
typedef void (*uart_recv_event_cb_t)(uart_data_t *data);

int zj_uart_recv_event_init(uart_recv_event_cb_t cb);
int zj_uart_send_event(uint8_t *data,int len);


void zj_uart_send_data(uint8_t *data,int len);
void zj_uart_init(uint8_t uart0_tx_pin,uint8_t uart0_rx_pin,uint32_t uart1_baudrate);
void zj_uart_baudrate_change(uint32_t uart_baudrate);

void hal_reboot();

zj_reset_reason_t zj_get_reset_reason();

#ifndef CONFIG_MAX_PIXEL_NUM
#define CONFIG_MAX_PIXEL_NUM  300
#endif

typedef struct{
   
   uint8_t R;
   uint8_t G;
   uint8_t B;
}pixel_payload_t;

int zj_pixel_init();
int zj_pixel_send(pixel_payload_t rgb[],uint16_t len);

#endif
