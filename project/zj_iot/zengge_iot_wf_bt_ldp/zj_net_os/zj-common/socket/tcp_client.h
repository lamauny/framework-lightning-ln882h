/* tcp_perf Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/


#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#ifdef __cplusplus
extern "C" {
#endif

#define TCPC_RECONNECT_TIME            3000  //msec
#define MONITOR_TIME_OUT_SEC           3
#define MONITOR_TIME_COUNT             3
#define TCPC_DISC_2_RST_STA_TIMEOUT    5  * 60  /* tcp 连接失败后重启sta /sec*/

#define LOGIN_PKT_LENGTH               320 

#ifdef CONFIG_USE_TCP_CLIENT_SOCKET
#define USE_TCP_CLIENT_SOCKET
#endif

#ifndef USE_TCP_CLIENT_SOCKET
#define USE_TCP_CLIENT_RAW
#endif

#ifndef CONFIG_TCPC_RECV_BUF_SIZE 
#define CONFIG_TCPC_RECV_BUF_SIZE    1460
#endif

typedef enum{
  
  MONITOR_TYPE_NONE ,
  MONITOR_TYPE_DNS,
  MONITOR_TYPE_CONNECT,
  MONITOR_TYPE_LOGIN ,      /*Monitor whether login is successful*/
  MONITOR_TYPE_LOGIN_RESPONSE,/*response successful*/
  MONITOR_TYPE_KEEPALIVE,   /*Monitor whether the heartbeat package responds*/
  MONITOR_TYPE_KEEPALIVE_RESPONSE,/*response successful*/
  MONITOR_TYPE_RECONNECT,
  MONITOR_TYPE_DISCONNECT,
  MONITOR_TYPE_RESET  ,
  MONITOR_TYPE_READY_CONNECT
}monitor_event_t;

void tcp_client_update_monitor_type(monitor_event_t type);

int  tcp_client_init();

void tcp_client_set_app_verinfo(uint32_t ver);
void tcp_client_clear_app_verinfo();
void tcp_client_set_uuid(char *id,int len);
void tcp_client_get_uuid(char *id);

int  tcp_client_set_config(char *host,int len,uint16_t port);
void tcp_client_get_config(char *host,int *len,uint16_t* port);

void tcp_client_set_heartbeat_time(int time);
char *tcp_get_dns_parse_addr();
int  tcp_client_start();
int  tcp_client_kill();
void tcp_client_restart();

int  tcp_client_send(unsigned char *pdata,int len);
int  tcp_client_get_connect_status(int xTicksToWait);
int  tcp_client_get_switch();

uint32_t tcp_client_get_login_count();
uint16_t tcp_client_get_poweron_count();

uint32_t WIFI_disconnect_count_get();
uint32_t WIFI_disconnect_time_get();

int tcp_client_ping_check();
void tcp_client_ping_set_success();
int tcp_client_ping_rusult(uint32_t wait_time);

void tcp_client_get_login_pkt(char *pkt);


#ifdef __cplusplus
}
#endif

#endif /*#ifndef __TCP_PERF_H__*/

