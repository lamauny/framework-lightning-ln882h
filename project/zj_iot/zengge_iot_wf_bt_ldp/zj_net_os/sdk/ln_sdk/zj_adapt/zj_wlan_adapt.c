
#include "zj_adapt_api.h"
#include "zj_adapt_config.h"

#define   TAG  "WFAPT"

static uint32_t g_restart_time = 1000;
esp_netif_t *sta_netif = NULL;
esp_netif_t *ap_netif = NULL;
static uint8_t g_wifi_state = WIFI_STATE_IDLE;

static bool g_wifi_sta_stop_flag;

void zj_wifi_STA_Start(uint8_t *ssid,uint8_t ssid_len,uint8_t *pwd,uint8_t pwd_len)
{
    g_wifi_state = WIFI_STATE_CONNECTING;
    wifi_mode_t mode;
    esp_wifi_get_mode(&mode);
    wifi_config_t wifi_config = {
        .sta = {
           // .threshold.authmode = WIFI_AUTH_WPA2_WPA3_PSK,
           // .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
        },
    };
    memcpy(wifi_config.sta.ssid,ssid,ssid_len);
    memcpy(wifi_config.sta.password,pwd,pwd_len);
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "wifi_cur mode %d",mode);
    if(mode == WIFI_MODE_APSTA || g_wifi_sta_stop_flag){

        esp_wifi_connect();
    }
    g_wifi_sta_stop_flag = false;
    ESP_LOGI(TAG, "wifi_init_sta finished");
}


void zj_wifi_STA_Stop()
{
    g_wifi_sta_stop_flag = true;
    esp_wifi_disconnect();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    g_wifi_state = WIFI_STATE_IDLE;
}

int wifi_mgmr_status_code_clean_internal()
{
    return 0;
}

static void __change_ip(esp_netif_t *netif)
{
    esp_err_t err = esp_netif_dhcps_stop(netif);
    if (err  != ESP_OK) {
        
        ESP_LOGE(TAG, "Failed to stop dhcp client %x",err);
        return;
    }
    esp_netif_ip_info_t ip;
    memset(&ip, 0 , sizeof(esp_netif_ip_info_t));
    IP4_ADDR(&ip.ip, 10, 10 , 123, 3);
    IP4_ADDR(&ip.gw, 10, 10 , 123, 3);
    IP4_ADDR(&ip.netmask, 255, 255 , 255, 0);
    if (esp_netif_set_ip_info(netif, &ip) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set ip info");
        return;
    }
    if (esp_netif_dhcps_start(netif)  != ESP_OK) {
        
        ESP_LOGE(TAG, "Failed to start dhcp client %x",err);
    }
}


void zj_wifi_AP_Start(uint8_t *ssid,uint8_t ssid_len,uint8_t *pwd,uint8_t pwd_len)
{
    wifi_config_t wifi_config = {
        .ap = {
            .authmode = WIFI_AUTH_WPA_WPA2_PSK,
            .pmf_cfg = {
                    .required = false,
            },
        },
    };
    memcpy(wifi_config.ap.ssid,ssid,ssid_len);
    wifi_config.ap.ssid_len = ssid_len;
    
    wifi_config.ap.max_connection = 4;
    if (pwd_len == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }else{
        memcpy(wifi_config.ap.password,pwd,pwd_len);
    }
    srand(PORT_CONFIG_SYSTEM_TIME);
	wifi_config.ap.channel = rand() % 10 + 1;
    __change_ip(ap_netif);
	ESP_LOGI(TAG,"wifi_init_ap finished.SSID:%s password:%s channel %d",
	ssid, pwd,wifi_config.ap.channel);
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start()); 
}

void zj_wifi_AP_Stop()
{
	
}


static TimerHandle_t g_reboot_timer;

static void reboot_timer_cb(void *arg)
{
   esp_restart();
}

void zj_restart_set_time(uint32_t time)
{
	g_restart_time = time;
}
/** 
 * @brief 1s后重启系统
 * @note  
 * @param[in] none
 * @return none
 */
int zj_restart_system(void)
{

	 if(g_reboot_timer == NULL){

		  g_reboot_timer = xTimerCreate("rst_tmr", g_restart_time / portTICK_PERIOD_MS, 0,(void *)0,(void *)reboot_timer_cb);
	    xTimerStart(g_reboot_timer,0);
   }
	return 0;
}


void zj_scan_router(zj_adapter_evt_t evt)
{

    uint16_t ap_count = 0;
    uint8_t real_ap_count = 0;
    esp_wifi_scan_start(NULL, true);
    
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
    if (ap_count == 0) {
       
       return;
    }
    zj_wifi_scan_inf_t *wifi_scan_buff = NULL;
    wifi_ap_record_t *ap_info = malloc(ap_count * sizeof(wifi_ap_record_t));
    for(int i = 0;i < ap_count;i++){

        memset(&ap_info[i],0,sizeof(wifi_ap_record_t));
    }
    if (ap_info == NULL) {
        ESP_LOGE(TAG, "Failed to malloc buffer to print scan results");
        return;
    }
    if((esp_wifi_scan_get_ap_records(&ap_count, ap_info)) == ESP_OK){

        ESP_LOGI(TAG, "Total APs scanned = %u", ap_count);
        if(ap_count){
            
            wifi_scan_buff = malloc(ap_count * sizeof(zj_wifi_scan_inf_t));
            if (wifi_scan_buff == NULL) {
                ESP_LOGE(TAG, "Failed to malloc buffer to print scan results");
                return;
            }
            for(int i = 0;i < ap_count;i++){

                memset(&wifi_scan_buff[i],0,sizeof(zj_wifi_scan_inf_t));
            }
            for (int i = 0; i < ap_count; i++) {
                
                if(memcmp((char *)ap_info[i].ssid,"LEDnet",strlen("LEDnet")) == 0 ||
                    strlen((char *)ap_info[i].ssid) == 0){

                    continue;
                }
                
                int ssid_len = strlen((char *)ap_info[i].ssid);
                memcpy(wifi_scan_buff[real_ap_count].ssid,ap_info[i].ssid, ssid_len <= 32 ? ssid_len : 32);
                memcpy(wifi_scan_buff[real_ap_count].bssid,ap_info[i].bssid,6);
                wifi_scan_buff[real_ap_count].channel = ap_info[i].primary;
                wifi_scan_buff[real_ap_count].rssi = ap_info[i].rssi;
                wifi_scan_buff[real_ap_count].authmode = ap_info[i].authmode;
                zj_printf("scan:%s ch%d %%%d auth%d\n",
                    (char *)wifi_scan_buff[real_ap_count].ssid,
                    wifi_scan_buff[real_ap_count].channel,
                    wifi_scan_buff[real_ap_count].rssi,
                    wifi_scan_buff[real_ap_count].authmode);
                real_ap_count++;
            }
            
            if(real_ap_count){

               zj_adapter_post_event(evt,wifi_scan_buff,NULL,real_ap_count);
            }
            
        }
    }

    free(ap_info);
    if(wifi_scan_buff){
        free(wifi_scan_buff);
    }
    ESP_LOGI(TAG,"real scan ap %d",real_ap_count);
}


void zj_wifi_get_ip_info(char *ip_addr)
{
  esp_netif_t *ifx = ap_netif;
  esp_netif_ip_info_t ip_info;
  wifi_mode_t mode;
  esp_wifi_get_mode(&mode);
  if (WIFI_MODE_STA == mode) {
  	
     ifx = sta_netif;
  }
  esp_netif_get_ip_info(ifx, &ip_info);
  sprintf(ip_addr, "%s", ip4addr_ntoa((ip4_addr_t *)&ip_info.ip.addr));
}

void zj_wifi_get_gateway(char *ip_addr)
{
  esp_netif_t *ifx = sta_netif;
  esp_netif_ip_info_t ip_info;
  esp_netif_get_ip_info(ifx, &ip_info);
  sprintf(ip_addr, "%s", ip4addr_ntoa((ip4_addr_t *)&ip_info.gw.addr));
}

ip_addr_t zj_wifi_get_LAN_broadcast_addr()
{
    esp_netif_t *ifx = sta_netif;
    esp_netif_ip_info_t ip_info;
    esp_netif_get_ip_info(ifx, &ip_info);

    ip4_addr_t ip;
    ip.addr = ip_info.ip.addr; 
    ip.addr |= 0xFF000000; 
    return ip;
}

ip_addr_t zj_wifi_get_ip_address()
{
    esp_netif_t *ifx = sta_netif;
    esp_netif_ip_info_t ip_info;
    esp_netif_get_ip_info(ifx, &ip_info);

    ip4_addr_t ip;
    ip.addr = ip_info.ip.addr;
    return ip;
}

void zj_wifi_set_ip_info(char *ip_addr)
{

}

int zj_wifi_get_mac_info(char *mac_str)
{
  uint8_t mac_addr[6] = {0};
  esp_wifi_get_mac(ESP_IF_WIFI_AP,mac_addr);
  sprintf(mac_str,"%02X%02X%02X%02X%02X%02X",mac_addr[0],mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5]);
  return 0;
}

int zj_wifi_get_mac_hex(uint8_t *mac_hex)
{
  esp_wifi_get_mac(ESP_IF_WIFI_AP, mac_hex);
  return 0;
}


int zj_wifi_get_state()
{
	return g_wifi_state;
}

int zj_get_ap_rssi()
{
    wifi_ap_record_t ap_info;
    esp_wifi_sta_get_ap_info(&ap_info);
	return ap_info.rssi;
}


uint8_t zj_get_lan_code()
{
    return 0;
}

static TimerHandle_t g_timer;
static void __got_ip_timeout()
{
    if(g_wifi_state != WIFI_STATE_CONNECTED_IP_GOT){
   
       ESP_LOGW(TAG,"got_ip_timeout");
       esp_wifi_disconnect();
    }
}


static void __got_ip_timeout_handler()
{
    if(g_timer == NULL){

        g_timer = xTimerCreate("gotip_tmr", 10000 / portTICK_PERIOD_MS, 0,(void *)0,__got_ip_timeout);
        xTimerStart(g_timer,0);
    }else{
        xTimerReset(g_timer,0);
    } 
}

static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    ESP_LOGI(TAG, "wifi base %s evt %ld",event_base,event_id);
    if (event_base == WIFI_EVENT){
        
        if(event_id == WIFI_EVENT_STA_START) {

           wifi_mode_t mode;
           esp_wifi_get_mode(&mode);
           if(mode == WIFI_MODE_STA){

              esp_wifi_connect();
              ESP_LOGI(TAG, "connect to the AP");
           }
        }else if(event_id == WIFI_EVENT_STA_CONNECTED){
            ESP_LOGI(TAG, "WIFI_EVENT_STA_CONNECTED");
           __got_ip_timeout_handler();
        }else if(event_id == WIFI_EVENT_STA_DISCONNECTED){
           
            wifi_event_sta_disconnected_t *disconnected = (wifi_event_sta_disconnected_t *)event_data; 
            ESP_LOGI(TAG, "retry to connect to the AP %d",disconnected->reason);
            zj_adapter_post_event(ADAPT_EVT_WIFI_DISCONNECTED,NULL,NULL,0);
            if(!g_wifi_sta_stop_flag){
                
               esp_wifi_connect();
            }
            switch (disconnected->reason)
            {
            case WIFI_REASON_AUTH_EXPIRE:
            case WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT:
            case WIFI_REASON_AUTH_FAIL:
            case WIFI_REASON_HANDSHAKE_TIMEOUT:
            case WIFI_REASON_MIC_FAILURE:
                ESP_LOGE(TAG, "STA Auth Error");
                g_wifi_state = WIFI_STATE_PSK_ERROR;
                break;
            case WIFI_REASON_NO_AP_FOUND:
                ESP_LOGE(TAG, "STA AP Not found");
                 g_wifi_state = WIFI_STATE_NO_AP_FOUND;
                break;
            default:
                g_wifi_state = WIFI_STATE_DISCONNECT;
                break;
            }
        }else if (event_id == WIFI_EVENT_AP_START) {
  
        }else if (event_id == WIFI_EVENT_AP_STACONNECTED) {
            wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
            ESP_LOGI(TAG, "station "MACSTR" join, AID=%d",
                    MAC2STR(event->mac), event->aid);
        } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
            wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
            ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d",
                    MAC2STR(event->mac), event->aid);
        }
    }else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip: %d.%d.%d.%d gw: %d.%d.%d.%d",IP2STR(&event->ip_info.ip),IP2STR(&event->ip_info.gw));
        if(event->ip_info.gw.addr == 0){
            ESP_LOGE(TAG, "gateway error");
            esp_wifi_disconnect();
        }else{
            zj_adapter_post_event(ADAPT_EVT_WIFI_CONNECTED,NULL,NULL,0);
            g_wifi_state = WIFI_STATE_CONNECTED_IP_GOT;
        }
    }
}

void zj_wifi_drv_init()
{
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_got_ip));
    zj_adapter_post_event(ADAPT_EVT_WIFI_INIT_DONE,NULL,NULL,0);                              
}
			