#include "zj_adapt_config.h"
#include "debug/log.h"

extern int aes_key_init();
extern void zj_adapter_init();;
extern void sys_dbg_init(); 
extern void net_type_init();
extern void zj_watchdog_start();

void zengge_common_main_init()
{
    /* init wifi */
    // ESP_ERROR_CHECK(esp_netif_init());
    // ESP_ERROR_CHECK(esp_event_loop_create_default());
    // sta_netif = esp_netif_create_default_wifi_sta();
    // ap_netif = esp_netif_create_default_wifi_ap();
    // wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    // ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    aes_key_init();
    zj_adapter_init();/* 适配器初始化初始化 */
    net_type_init();  /* 网络类型初始化 */
#ifdef CONFIG_WIFI_BEST_PERFORMANCE_OPTION
    extern uint8_t net_type_get_config();
    if(net_type_get_config() != 0){
    //     esp_wifi_set_ps(WIFI_PS_NONE);
    //     ESP_LOGW("ps","WIFI_PS_NONE");
    }
#endif
    zj_watchdog_start();
}
