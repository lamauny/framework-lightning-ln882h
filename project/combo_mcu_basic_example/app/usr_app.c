#include "osal/osal.h"
#include "utils/debug/log.h"
#include "utils/debug/ln_assert.h"
#include "utils/system_parameter.h"
#include "utils/ln_psk_calc.h"
#include "utils/power_mgmt/ln_pm.h"
#include "utils/sysparam_factory_setting.h"
#include "wifi.h"
#include "wifi_port.h"
#include "netif/ethernetif.h"
#include "wifi_manager.h"
#include "lwip/tcpip.h"
#include "drv_adc_measure.h"
#include "hal/hal_adc.h"
#include "ln_nvds.h"
#include "ln_wifi_err.h"
#include "ln_misc.h"
#include "ln882h.h"


#include "rwip_config.h"
#include "llm_int.h"

#include "ln_ble_gap.h"
#include "ln_ble_gatt.h"
#include "usr_app.h"
#include "usr_ble_app.h"

#include "lwip/dns.h"
#include "lwip/tcpip.h"
#include "lwip/ip.h"
#include "lwip/api.h"
#include "lwip/sockets.h"

static OS_Thread_t g_usr_app_thread;
#define USR_APP_TASK_STACK_SIZE   6*256 //Byte

#define WIFI_TEMP_CALIBRATE             1//1

#if WIFI_TEMP_CALIBRATE
static OS_Thread_t g_temp_cal_thread;
#define TEMP_APP_TASK_STACK_SIZE   4*256 //Byte
#endif

/* declaration */
static void wifi_init_ap(void);
static void wifi_init_sta(void);
static void usr_app_task_entry(void *params);
static void temp_cal_app_task_entry(void *params);

static uint8_t mac_addr[6]        = {0x00, 0x50, 0xC2, 0x5E, 0xAA, 0xDA};
static uint8_t psk_value[40]      = {0x0};
// static uint8_t target_ap_bssid[6] = {0xC0, 0xA5, 0xDD, 0x84, 0x6F, 0xA8};

wifi_sta_connect_t connect = {
    .ssid    = "LN_LAB_TEST",
    .pwd     = "12345678",
    .bssid   = NULL,
    .psk_value = NULL,
};

wifi_scan_cfg_t scan_cfg = {
        .channel   = 0,
        .scan_type = WIFI_SCAN_TYPE_ACTIVE,
        .scan_time = 20,
};

wifi_softap_cfg_t ap_cfg = {
    .ssid            = "LN_AP_8899",
    .pwd             = "12345678",
    .bssid           = mac_addr,
    .ext_cfg = {
        .channel         = 6,
        .authmode        = WIFI_AUTH_WPA_WPA2_PSK,//WIFI_AUTH_OPEN,
        .ssid_hidden     = 0,
        .beacon_interval = 100,
        .psk_value = NULL,
    }
};

static void wifi_scan_complete_cb(void * arg)
{
    LN_UNUSED(arg);

    ln_list_t *list;
    uint8_t node_count = 0;
    ap_info_node_t *pnode;

    wifi_manager_ap_list_update_enable(LN_FALSE);

    // 1.get ap info list.
    wifi_manager_get_ap_list(&list, &node_count);

    // 2.print all ap info in the list.
    LN_LIST_FOR_EACH_ENTRY(pnode, ap_info_node_t, list,list)
    {
        uint8_t * mac = (uint8_t*)pnode->info.bssid;
        ap_info_t *ap_info = &pnode->info;

//        LOG(LOG_LVL_INFO, "\tCH=%2d,RSSI= %3d,", ap_info->channel, ap_info->rssi);
//        LOG(LOG_LVL_INFO, "BSSID:[%02X:%02X:%02X:%02X:%02X:%02X],SSID:\"%s\"\r\n", \
//                           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], ap_info->ssid);
    }

    wifi_manager_ap_list_update_enable(LN_TRUE);
}

static void wifi_init_sta(void)
{
    // ln_generate_random_mac(mac_addr);
    //1. sta mac get
     if (SYSPARAM_ERR_NONE != sysparam_sta_mac_get(mac_addr)) {
        LOG(LOG_LVL_ERROR, "[%s]sta mac get failed!!!\r\n", __func__);
        return;
    }

    if (mac_addr[0] == STA_MAC_ADDR0 &&
        mac_addr[1] == STA_MAC_ADDR1 &&
        mac_addr[2] == STA_MAC_ADDR2 &&
        mac_addr[3] == STA_MAC_ADDR3 &&
        mac_addr[4] == STA_MAC_ADDR4 &&
        mac_addr[5] == STA_MAC_ADDR5) {
        ln_generate_random_mac(mac_addr);
        sysparam_sta_mac_update((const uint8_t *)mac_addr);
    }

    //1. net device(lwip)
    netdev_set_mac_addr(NETIF_IDX_STA, mac_addr);
    netdev_set_active(NETIF_IDX_STA);
    sysparam_sta_mac_update((const uint8_t *)mac_addr);

    //2. wifi start
    wifi_manager_reg_event_callback(WIFI_MGR_EVENT_STA_SCAN_COMPLETE, &wifi_scan_complete_cb);

    if(WIFI_ERR_NONE != wifi_sta_start(mac_addr, WIFI_NO_POWERSAVE)){
        LOG(LOG_LVL_ERROR, "[%s]wifi sta start filed!!!\r\n", __func__);
    }

    connect.psk_value = NULL;
    if (strlen(connect.pwd) != 0) {
        if (0 == ln_psk_calc(connect.ssid, connect.pwd, psk_value, sizeof (psk_value))) {
            connect.psk_value = psk_value;
            hexdump(LOG_LVL_INFO, "psk value ", psk_value, sizeof(psk_value));
        }
    }

    wifi_sta_connect(&connect, &scan_cfg);
}

static void ap_startup_cb(void * arg)
{
    netdev_set_state(NETIF_IDX_AP, NETDEV_UP);
}

static void wifi_init_ap(void)
{
    tcpip_ip_info_t  ip_info;
    server_config_t  server_config;

    ip_info.ip.addr      = ipaddr_addr((const char *)"192.168.4.1");
    ip_info.gw.addr      = ipaddr_addr((const char *)"192.168.4.1");
    ip_info.netmask.addr = ipaddr_addr((const char *)"255.255.255.0");

    server_config.server.addr   = ip_info.ip.addr;
    server_config.port          = 67;
    server_config.lease         = 2880;
    server_config.renew         = 2880;
    server_config.ip_start.addr = ipaddr_addr((const char *)"192.168.4.100");
    server_config.ip_end.addr   = ipaddr_addr((const char *)"192.168.4.150");
    server_config.client_max    = 3;
    dhcpd_curr_config_set(&server_config);

    //1. net device(lwip).
    netdev_set_mac_addr(NETIF_IDX_AP, mac_addr);
    netdev_set_ip_info(NETIF_IDX_AP, &ip_info);
    netdev_set_active(NETIF_IDX_AP);
    wifi_manager_reg_event_callback(WIFI_MGR_EVENT_SOFTAP_STARTUP, &ap_startup_cb);

    sysparam_softap_mac_update((const uint8_t *)mac_addr);

    ap_cfg.ext_cfg.psk_value = NULL;
    if ((strlen(ap_cfg.pwd) != 0) &&
        (ap_cfg.ext_cfg.authmode != WIFI_AUTH_OPEN) &&
        (ap_cfg.ext_cfg.authmode != WIFI_AUTH_WEP)) {
        memset(psk_value, 0, sizeof(psk_value));
        if (0 == ln_psk_calc(ap_cfg.ssid, ap_cfg.pwd, psk_value, sizeof (psk_value))) {
            ap_cfg.ext_cfg.psk_value = psk_value;
            hexdump(LOG_LVL_INFO, "psk value ", psk_value, sizeof(psk_value));
        }
    }

    //2. wifi
    if(WIFI_ERR_NONE !=  wifi_softap_start(&ap_cfg)){
        LOG(LOG_LVL_ERROR, "[%s, %d]wifi_start() fail.\r\n", __func__, __LINE__);
    }
}

static OS_Semaphore_t wait_dns_sem;
static ip_addr_t      my_http_ip;

static void dns_found(const char *name, ip_addr_t *host_ip, void *callback_arg)
{
    OS_SemaphoreRelease(&wait_dns_sem);
    my_http_ip.addr = host_ip->addr;
}

/**
 * @brief get ip address by lwip dns
 * 
 * @param hostname host name
 * @param ip host ip address
 * @param cb dns found call back function
 * @return int return execution result
 */
static int ln_drv_get_ip_by_dns(char *hostname,ip_addr_t *ip,dns_found_callback cb)
{
    OS_Status ret_sta = OS_SemaphoreCreateBinary(&wait_dns_sem);
    if (ret_sta != OS_OK) {
        LOG(LOG_LVL_ERROR, "Wait DNS sem creat fail\r\n");
        return HAL_ERROR;
    }
    err_t ret = dns_gethostbyname(hostname, ip, cb,NULL);;
    if (ret == ERR_INPROGRESS){
      
    }else if (ret == ERR_OK){
     
        OS_SemaphoreRelease(&wait_dns_sem);
    }else if(ret == ERR_OK){
        LOG(LOG_LVL_ERROR, "Get dns result err.Error code:%d\n",ret);
        return HAL_ERROR;
    }
    OS_SemaphoreWait(&wait_dns_sem, 30000);
    if(OS_SemaphoreGetCount(&wait_dns_sem) == 0){
        ip->addr = my_http_ip.addr;
        return HAL_OK;
    }else{
        LOG(LOG_LVL_ERROR, "Get dns result timeout.\n");
        return HAL_ERROR;
    }
}

/**
 * @brief http get request
 * 
 * @param host_name host name
 * @param url url,resource path
 * @param ret_str the http get return
 * @param ret_str_len the content returned by HTTP GET
 * @param ret_str_max_len the content length returned by HTTP GET
 * @return int return execution result
 */
int ln_drv_http_get(uint8_t *host_name,uint8_t *url,uint8_t *ret_str,uint32_t *ret_str_len,uint32_t ret_str_max_len)
{
    //get host ip by dns
    ip_addr_t host_ip;
    host_ip.addr = 0;
    if(ln_drv_get_ip_by_dns((char*)host_name,&host_ip,(dns_found_callback)dns_found) != HAL_OK){
        LOG(LOG_LVL_ERROR, "Get HOST IP failed.\n");
        return HAL_ERROR;
    }else{
        LOG(LOG_LVL_INFO, "HOST IP:%s.\n",ip4addr_ntoa(&(host_ip.addr)));
    }

    //tcp connect by lwip socket
    int     ret  = 0;
    struct  sockaddr_in  client_addr;
    int     sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock < 0){
        LOG(LOG_LVL_ERROR, "Socket init failed\n");
        return HAL_ERROR;
    }
    memset(&client_addr,0,sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(80);
    client_addr.sin_addr.s_addr = host_ip.addr;
    memset(&(client_addr.sin_zero), 0, sizeof(client_addr.sin_zero));
    
    for(int i = 0; i < 10; i ++){
        ret = connect(sock,(struct sockaddr *)&client_addr,sizeof(struct sockaddr));
        if(ret != -1)
            break;
        OS_MsDelay(50);
    }
    
    if(ret == -1){
        LOG(LOG_LVL_ERROR, "TCP Connect failed!\n");
        closesocket(sock);
        OS_MsDelay(10);
        return HAL_ERROR;
    }else{
        ret = write(sock,url,strlen((char*)url));
        if(ret != strlen((char*)url)){
            LOG(LOG_LVL_ERROR, "TCP write data failed!\n");
            closesocket(sock);
            return HAL_ERROR;
        }
        ret = recv(sock, ret_str,ret_str_max_len,0);
        closesocket(sock);
        if(ret <= 0){
            LOG(LOG_LVL_ERROR, "TCP read data failed!\n");
            closesocket(sock);
            return HAL_ERROR;
        }else{
            *ret_str_len = ret;
        }
    }
    closesocket(sock);
    return HAL_OK;
}
static OS_Thread_t g_get_net_data_thread;
static uint8_t my_weather_code = 0;
static int8_t  my_weather_tem  = 0;
static uint8_t my_position_str[50] = {0};
static uint8_t cur_status      = 0;
static void get_net_data_task_entry(void *params);

static int find_information(uint8_t *data,uint8_t *weather_code,int8_t *weather_tem,uint8_t *position_str)
{
    char    *token = NULL; 
    char    *endptr = NULL; 
	char 	delimiters[] = ",";  	
    token = strtok((char*)data,delimiters);
    while(token != NULL){
        token = strtok(NULL,delimiters);
        if(memcmp("name",token+1,strlen("name")) == 0){
            strcpy((char*)position_str,token+8);
						position_str[strlen(position_str)-1] = 0x0;//delete '"'
        }
        if(memcmp("code",token+1,strlen("code")) == 0){
            my_weather_code = strtol(token+8,&endptr,0);
        }
        if(memcmp("temperature",token+1,strlen("temperature")) == 0){
            my_weather_tem = strtol(token+15,&endptr,0);
            return HAL_OK;
        }
    }
    return HAL_ERROR;
}
//http://api.seniverse.com/v3/weather/now.json?key=S9Sg1Ymshl8NeI68d&location=ip&language=zh-Hans&unit=c
#define WEATHER_HOST_NAME   "api.seniverse.com"
#define WEATHER_URL         "GET /v3/weather/now.json?key=S9Sg1Ymshl8NeI68d&location=ip&language=zh-Hans&unit=c\r\n\r\n"
#define WEATHER_HOST_IP     "116.62.81.138"
#define GET_NET_DATA_LEN    2048
static void usr_app_task_entry(void *params)
{
    LN_UNUSED(params);

    ln_pm_sleep_mode_set(ACTIVE);

    wifi_manager_init();

    wifi_init_sta();
    // wifi_init_ap();
    

    while (!netdev_got_ip()) {
        OS_MsDelay(1000);
    }
    uint32_t data_len = 0;
    uint8_t *ret_data = NULL;
    while (1)
    {
        data_len = 0;
        ret_data = OS_Malloc(GET_NET_DATA_LEN);
        memset(ret_data,0,GET_NET_DATA_LEN);
        if(HAL_OK == ln_drv_http_get((uint8_t*)&WEATHER_HOST_NAME,(uint8_t*)&WEATHER_URL,(uint8_t*)ret_data,&data_len,GET_NET_DATA_LEN)){
            LOG(LOG_LVL_INFO,"HTTP GET RET:%s",(uint8_t *)ret_data);
            if(HAL_OK == find_information(ret_data,&my_weather_code,&my_weather_tem,my_position_str)){
                cur_status = 1;
                OS_Free(ret_data);
				LOG(LOG_LVL_INFO,"GET WEATHER OK");
                ln_pm_sleep_frozen(20000,1,0,0,1);
            }
        }else{
            LOG(LOG_LVL_INFO,"Get weather information failed.");
        }
        OS_Free(ret_data);

//        //OS_MsDelay(1000 * 60 * 60);
        OS_MsDelay(2000);
        //ln_pm_sleep_frozen(5000,1,0,0,0);
    }
    while(1)
    {
        OS_MsDelay(1000);
        
    }
}

static void temp_cal_app_task_entry(void *params)
{
    LN_UNUSED(params);

    int8_t cap_comp = 0;
    uint16_t adc_val = 0;
    int16_t curr_adc = 0;
    uint8_t cnt = 0;

    if (NVDS_ERR_OK == ln_nvds_get_xtal_comp_val((uint8_t *)&cap_comp)) {
        if ((uint8_t)cap_comp == 0xFF) {
            cap_comp = 0;
        }
    }

    drv_adc_init();

    wifi_temp_cal_init(drv_adc_read(ADC_CH0), cap_comp);

    while (1)
    {
        OS_MsDelay(1000);

        adc_val = drv_adc_read(ADC_CH0);
        wifi_do_temp_cal_period(adc_val);

        curr_adc = (adc_val & 0xFFF);

        cnt++;
        if ((cnt % 60) == 0) {
            LOG(LOG_LVL_INFO, "adc raw: %4d, temp_IC: %4d\r\n",
                    curr_adc, (int16_t)(25 + (curr_adc - 770) / 2.54f));
            LOG(LOG_LVL_INFO, "Total:%d; Free:%ld;\r\n", 
                    OS_HeapSizeGet(), OS_GetFreeHeapSize());
        }
    }
}

void creat_usr_app_task(void)
{
    if(OS_OK != OS_ThreadCreate(&g_usr_app_thread, "WifiUsrAPP", usr_app_task_entry, NULL, OS_PRIORITY_BELOW_NORMAL, USR_APP_TASK_STACK_SIZE)) {
        LN_ASSERT(1);
    }

    ble_creat_usr_app_task();

#if  WIFI_TEMP_CALIBRATE
    if(OS_OK != OS_ThreadCreate(&g_temp_cal_thread, "TempAPP", temp_cal_app_task_entry, NULL, OS_PRIORITY_BELOW_NORMAL, TEMP_APP_TASK_STACK_SIZE)) {
        LN_ASSERT(1);
    }
#endif

    /* print sdk version */
    {
        LOG(LOG_LVL_INFO, "LN882H SDK Ver: %s [build time:%s][0x%08x]\r\n",
                LN882H_SDK_VERSION_STRING, LN882H_SDK_BUILD_DATE_TIME, LN882H_SDK_VERSION);
    }
}
