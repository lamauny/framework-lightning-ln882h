
# 征极WIFI sdk开发环境适配
## 目录结构
```
    📦surpl_wifi_ldp
    ┣ 📂zengge_app  /*应用程序 */
    ┣ 📂zj_net_os   
            ┣ 📂adapter_common  /* 链接中间层 */
            ┣ 📂sdk             /* 各类芯片厂商的sdk */
               ┣a_sdk
                  ┗adapter
               ┗b_sdk
                  ┗adapter
            ┗ 📂zj-common       /* 征极wifi iot设备通用组件 */
```
------------
## 组件依赖说明
```
  zengge_app->zj-common -> adapter_common->sdk
```
------------
## SDK编译适配
  - 客户需要把libzj_os.a和libzj_app.a库文件添加编译路径中。
  - 备注：目前只支持freeRTOS和lwip。

----------

## zj_net_os adapter接口适配
### 打印适配
-  #define zj_printf  
-  #define os_printf 

----------

### 适配应用层线程优先级
 - #define PORT_CONFIG_LIGHT_TASK_PRIO          
 - #define PORT_CONFIG_LIGHT_FUNC_PRIO           
 - #define PORT_CONFIG_LIGHT_REMOTE_TASK_PRIO   
- #define PORT_CONFIG_TCP_SERVER_TASK_PRIO      
- #define PORT_CONFIG_TCP_CLIENT_TASK_PRIO      
- #define PORT_CONFIG_TCP_CLIENT_SEND_TASK_PRIO 
- #define PORT_CONFIG_UDP_SERVER_TASK_PRIO     
- #define PORT_CONFIG_TIMER_CLOCK_TASK_PRIO    
- #define PORT_CONFIG_AUTO_REPORT_TASK_PRIO     
- #define PORT_CONFIG_FOTA_TASK_PRIO            
- #define PORT_CONFIG_ZG_EVENT_LOOP_TASK_PRIO  
- #define PORT_CONFIG_ZG_WATCHDOG_TASK_PRIO     
- #define PORT_CONFIG_KV_TASK_PRIO             
- #define PORT_CONFIG_MIC_TASK_PRIO            
- #define PORT_CONFIG_BT_REMOTE_EVENT_TASK_PRIO
- #define PORT_CONFIG_KEY_TASK_PRIO            
- #define PORT_CONFIG_BLE_TASK_PRIO             
- #define ZG_DATA_TASK_PRIO                    
- #define PORT_CONFIG_NET_DIAGNOSIS_TASK_PRIO  

----------

### 适配系统时间 
- PORT_CONFIG_SYSTEM_TIME

----------

### flash分区设置

- PROT_CONFIG_KEY_FLASH_ADDR：适配密钥储存的flash地址,1个扇区。
- PORT_CONFIG_CIPHERTEXT_FLASH_ADDR：适配密文储存的flash地址,1个扇区。
- PORT_CONFIG_USER_DATA_FLASH_ADDR：适配用户存数据存储区。
- PORT_CONFIG_USER_DATA_SIZE：适配用户存储数据flash大小，大于或等于3个扇区以上。
- PORT_BTR_NEW_SCENE_ADDR：适配场景功能存储flash地址,1个扇区。
- **备注：需要往PROT_CONFIG_KEY_FLASH_ADDR烧录征极提供的密钥**
----------

### 适配ota文件名称
- PORT_CONFIG_OTA_IMAGE_NAME

----------

### Storage
> Flash操作的接口，需要不同的平台对其进行实现。
----------
 - void zj_flash_write(uint32_t addr , uint8_t *pbuf,int len);
- 函数说明
> 向flash写入数据
- 参数

| 输入/输出 | 参数名 |描述 |
| ------- | ------- |------- |
|    [in]     |      addr   |    写入的flash首地址    |
|    [in]     |      pbuf   |    写入数据的buffer指针    |
|    [in]     |      len   |    写入的buffer大小    |
- 返回值
> 无
- 备注
> 写一个扇区时间在20ms以内。


----------

- void zj_flash_read(uint32_t addr , uint8_t *pbuf,int len);
- 函数说明
> 从flash读取数据
- 参数

| 输入/输出 | 参数名 |描述 |
| ------- | ------- |------- |
|    [in]     |      addr   |    读取的flash首地址    |
|    [out]     |      pbuf   |    读取数据的buffer指针    |
|    [in]     |      len   |   读取的buffer大小    |
- 返回值
> 无
- 备注
1. 由于上层业务可能出现多线程同时调用flash层的api接口，因此zj_flash_write，zj_flash_read，zj_flash_erase三个操作函数需要共用互斥锁。
若调用的芯片flash api本身具有互斥性，则无需再加锁；若调用的芯片flash api本身不具互斥性，则需在adapter_flash层添加互斥锁。
1. 如果支持片上运行，在操作flash可能会导致系统tick不准，在操作flash后需要弥补系统tick（重要）。
1. 读一个扇区时间在3ms以内。

----------

 - void zj_flash_erase(uint32_t addr,uint32_t size);
- 函数说明
> 从flash读取数据
- 参数

| 输入/输出 | 参数名 |描述 |
| ------- | ------- |------- |
|    [in]     |      addr   |    擦除的flash首地址    |
|    [in]     |      len   |    擦除的flash大小    |
- 返回值
> 无
- 备注
> 擦一个扇区时间在65ms以内。

----------

 - void zj_userdata_delete_key(char *key);
- 函数说明
> 删除kv存储库键值。
- 参数

| 输入/输出 | 参数名 |描述 |
| ------- | ------- |------- |
|    [in]     |      key   |    键值    |
- 返回值
> 无

----------

 - int zj_userdata_write(char *key, uint8_t *dat,int len);
- 函数说明
> 用户存储数据：kv存储函数，每一个键值分配一个存储空间。
- 参数

| 输入/输出 | 参数名 |描述 |
| ------- | ------- |------- |
|    [in]     |      key   |    键值    |
|    [in]     |      dat   |    写入数据的buffer指针    |
|    [in]     |      len   |    写入的buffer大小    |
- 返回值
> 返回写入的字节数
- 备注
> kv数据库，要求：磨损平衡、掉电保护。


----------

 - int zj_userdata_read(char *key, uint8_t *dat,int len);
- 函数说明
> kv读取函数。
- 参数

| 输入/输出 | 参数名 |描述 |
| ------- | ------- |------- |
|    [in]     |      key   |    键值    |
|    [out]     |      dat   |    读取数据的buffer指针    |
|    [in]     |      len   |    读取的buffer大小    |
- 返回值
> 返回读取的字节数


----------

### PWM
> PWM接口适配。

----------
- 通道适配：
```
- #define PORT_CONFIG_PWM_RED_CHANNEL           ：适配pwm red channel
- #define PORT_CONFIG_PWM_GREEN_CHANNEL         ：适配pwm green channel
- #define PORT_CONFIG_PWM_BLUE_CHANNEL          ：适配pwm blue channel
- #define PORT_CONFIG_PWM_WARM_CHANNEL          ：适配pwm warm channel
 - #define PORT_CONFIG_PWM_COLD_CHANNEL         ：适配pwm cold channel
```
----------

 - void zj_pwm_init();
 - 函数说明
> pwm初始化函数。
- 返回值
> 无

----------

 - void zj_pwm_red_channel_init(int freq);
- 函数说明
> pwm 红色通道初始化。
- 参数

| 输入/输出 | 参数名 |描述 |
| ------- | ------- |------- |
|    [in]     |      freq   |    频率：单位hz    |
- 返回值
> 无

----------

 - void zj_pwm_green_channel_init(int freq);
- 函数说明
> pwm 绿色通道初始化。
- 参数

| 输入/输出 | 参数名 |描述 |
| ------- | ------- |------- |
|    [in]     |      freq   |    频率：单位hz    |
- 返回值
> 无

----------

 - void zj_pwm_blue_channel_init(int freq);
- 函数说明
> pwm 蓝色通道初始化。
- 参数

| 输入/输出 | 参数名 |描述 |
| ------- | ------- |------- |
|    [in]     |      freq   |    频率：单位hz    |
- 返回值
> 无

----------

 - void zj_pwm_warm_channel_init(int freq);
- 函数说明
> pwm 暖光通道初始化。
- 参数

| 输入/输出 | 参数名 |描述 |
| ------- | ------- |------- |
|    [in]     |      freq   |    频率：单位hz    |
- 返回值
> 无

----------

 - void zj_pwm_cold_channel_init(int freq);
- 函数说明
> pwm 冷光通道初始化。
- 参数

| 输入/输出 | 参数名 |描述 |
| ------- | ------- |------- |
|    [in]     |      freq   |    频率：单位hz    |
- 返回值
> 无

----------

 - void zj_pwm_set_duty(uint8_t ch,int duty);
 - 函数说明
> pwm占空比设置

- 参数

| 输入/输出 | 参数名 |描述 |
| ------- | ------- |------- |
|    [in]     |      ch   |    通道    |
|    [in]     |      duty   |    占空比（0~25500）    |
- 返回值
> 无

----------

### watch dog
> watch dog接口适配。

----------
 - void zj_watchdog_start();
 - 函数说明
> 开启看门狗功能
- 返回值
> 无
 - int zj_watchdog_stop(uint32_t time);
- 函数说明
> 关闭看门狗功能
- 返回值
> 无

### ota
> ota接口适配。

----------

 - int zj_ota_get_image(char *http_url);
 - 函数说明
> 升级开始函数
- 参数

| 输入/输出 | 参数名 |描述 |
| ------- | ------- |------- |
|    [in]     |      http_url   |    http ota文件链接    |

- 返回值
> 0：成功，其它失败

- 备注
> 需要调用zj_adapter_post_event(ADAPT_EVT_OTA_STATUS,状态码，？），推送ota的状态，具体参考例程。
- 状态码：

```
typedef enum {
    UPGRADE_FAIL = -1,
    UPGRADE_PROGRESS = 0,
    UPGRADE_VER_ERR,
    UPGRADE_HTTP_FAIL,
    UPGRADE_TYPE_ERR,
    UPGRADE_ONGIONG,
    UPGRADE_AES_ERR,
    UPGRADE_MD5_ERR,
    UPGRADE_HEADER_ERR,
    UPGRADE_TIMEOUT,
    UPGRADE_SUCCESS = 100,
    UPGRADE_FILE_COMPLETE,
}fota_status_t;  
```

----------
 - int zj_ota_verify_image();
 - 函数说明
> 升级固件校验函数

- 返回值
> 0：成功，其它失败
 
----------
 - void zj_ota_reboot();
 - 函数说明
> reboo函数

- 返回值
> 无

### 重启函数

----------

 - int zj_restart_system(void);
 - 函数说明
> 芯片延时重启函数

- 返回值
> 0：成功，其它失败


----------
void hal_reboot();
 - 函数说明
> 芯片重启函数

- 返回值
> 无


----------
### RTC

----------
void zj_rtc_set_time(struct rtc_time *time_set);
 - 函数说明
> RTC时间设置

- 参数

| 输入/输出 | 参数名 |描述 |
| ------- | ------- |------- |
|    [in]     |      time_set   |    设置时间    |

- 返回值
> 无

----------

 - uint32_t zj_get_current_timestamp();
 - 函数说明
> 获取rtc当前时间戳

- 返回值
> 时间戳


----------

### free heap

----------

uint32_t zj_get_freeheap();
 - 函数说明
> 获取系统free heap

- 返回值
> free heap


----------

### Wi-Fi


 - void zj_wifi_STA_Start(uint8_t *ssid,uint8_t ssid_len,uint8_t *pwd,uint8_t pwd_len);
 - 函数说明
> 连接路由器

- 参数

| 输入/输出 | 参数名 |描述 |
| ------- | ------- |------- |
|    [in]     |      ssid   |    路由器ssid    |
|    [in]     |      ssid_len   |    ssid长度    |
|    [in]     |      pwd   |    密钥    |
|    [in]     |      pwd_len   |    密钥长度    |

- 返回值
> 无


----------

 - void zj_wifi_STA_Stop();
 - 函数说明
> 断开路由器

- 返回值
> 无


----------

 - void zj_wifi_AP_Start(uint8_t *ssid,uint8_t ssid_len,uint8_t *pwd,uint8_t pwd_len);
 - 函数说明
> 连接路由器

- 参数

| 输入/输出 | 参数名 |描述 |
| ------- | ------- |------- |
|    [in]     |      ssid   |    AP ssid    |
|    [in]     |      ssid_len   |    ssid长度    |
|    [in]     |      pwd   |    密钥    |
|    [in]     |      pwd_len   |    密钥长度    |

- 返回值
> 无


----------
 - void zj_wifi_AP_Stop();
 - 函数说明
> 关闭AP模式

- 返回值
> 无


----------
 - void zj_scan_router(zj_adapter_evt_t evt);
 - 函数说明
> 扫描附近AP
- 参数

| 输入/输出 | 参数名 |描述 |
| ------- | ------- |------- |
|    [in]     |      evt   |    触发的事件    |
- 返回值
> 无

----------

 - void zj_wifi_drv_init();
 - 函数说明
> wifi驱动初始化

- 返回值
> 无

----------

 - void zj_wifi_get_ip_info(char *ip_addr);
 - 函数说明
> 获取ip字符串

- 参数

| 输入/输出 | 参数名 |描述 |
| ------- | ------- |------- |
|    [out]     |      ip_addr   |    ip字符串    |
- 返回值
> 无
- 备注
> 当wifi处于sta模式时，ip地址为sta的ip地址，当wifi处于ap模式，则ip地址为ap的ip地址。

----------
 - void zj_wifi_get_gateway(char *ip_addr);
 - 函数说明
> 获取网关ip字符串

- 参数

| 输入/输出 | 参数名 |描述 |
| ------- | ------- |------- |
|    [out]     |      ip_addr   |    ip字符串    |
- 返回值
> 无

----------

int zj_wifi_get_mac_info(char *mac_str);
 - 函数说明
> 获取mac地址

- 参数

| 输入/输出 | 参数名 |描述 |
| ------- | ------- |------- |
|    [out]     |      mac_str   |    mac字符串    |
- 返回值
> 无
- 备注
> 取蓝牙广播的mac地址。

----------
int zj_wifi_get_mac_hex(uint8_t *mac_hex);
 - 函数说明
> 获取mac地址

- 参数

| 输入/输出 | 参数名 |描述 |
| ------- | ------- |------- |
|    [out]     |      mac_hex   |    mac[6] buffer    |
- 返回值
> 无
- 备注
> 取蓝牙广播的mac地址。

----------
 - int zj_wifi_get_state();
 - 函数说明
> 获取wifi当前状态

- 返回值
> wifi状态
- 备注
参考demo，状态码：
```

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

```
----------
int zj_get_ap_rssi();
 - 函数说明
> 获取连接路由器的后的rssi

- 返回值
> rssi

----------


### BLE
> GATT服务：

![](https://gitee.com/wayching_admin/zengge_iot_wf_bt_ldp/raw/master/doc/ble%20profile.png)

 - void zj_ble_drv_init();
 - 函数说明
> 蓝牙驱动初始化

- 返回值
> 无


----------

void zj_ble_scan_stop();

- 函数说明
> 蓝牙扫描停止

- 返回值
> 无


----------
void zj_ble_scan_start();
 - 函数说明
> 蓝牙扫描开启

- 返回值
> 无

- 备注：扫描间隔、周期参考demo

----------

- void zj_ble_adv_start();
- 函数说明
> 蓝牙广播开启

- 返回值
> 无

- 备注：
> 调用zj_adapter_get_adv_data_config()获取要设置的adv数据，调用zj_adapter_get_adv_scanResp_config（）获取要设置的扫描响应数据，调用zj_adapter_get_adv_name_config（）获取要设置的广播名称，细节参考demo。

----------

- void zj_ble_adv_stop();
 - 函数说明
> 蓝牙广播关闭

- 返回值
> 无

----------

- void zj_ble_notify(uint8_t *data,int len);
- 函数说明
> ble notify
- 参数

| 输入/输出 | 参数名 |描述 |
| ------- | ------- |------- |
|    [in]     |      data   |    发送数据buffer    |
|    [in]     |      len   |    数据长度    |
- 返回值
> 无

----------


- uint8_t zj_ble_get_connected_status();
 - 函数说明
> ble 连接状态

- 返回值
> 1：连接成功，其它没有连接

----------




- uint16_t zj_ble_get_mtu();
 - 函数说明
> 获取ble mtu

- 返回值
> mtu

----------


- uint8_t is_ble_init_finished();
 - 函数说明
> 蓝牙驱动是否初始化完毕

- 返回值
> 1：完毕，其它未完毕

----------

### 微妙延时


void zj_timer_delay_us(uint32_t us);

 - 函数说明
> 蓝牙驱动是否初始化完毕
- 参数

| 输入/输出 | 参数名 |描述 |
| ------- | ------- |------- |
|    [in]     |      us   |    微妙    |
- 返回值
> 无


zj_reset_reason_t zj_get_reset_reason();
 - 函数说明
> 获取重启原因
- 参数
- 返回值
> 重启原因
- 备注
```
typedef enum {
    RESET_RESON_UNKNOWN,
    RESET_RESON_POWER_ON,
    RESET_RESON_WDT,
    RESET_RESON_BROWN_OUT,
    RESET_RESON_SOFTWARE,
    RESET_RESON_HARDWARE,
    RESET_RESON_CRASH
}zj_reset_reson_t;  
```

----------