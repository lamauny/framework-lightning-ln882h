/* udp_perf Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/


#ifndef __UDP_PERF_H__
#define __UDP_PERF_H__

#include "lwip/ip_addr.h"

#ifdef __cplusplus
extern "C" {
#endif

#define UDP_PORT_DEFAULT        48899

#ifdef CONFIG_USE_UDP_SERVER_SOCKET
#define USE_UDP_SERVER_SOCKET
#endif

#ifndef USE_UDP_SERVER_SOCKET
#define USE_UDP_SERVER_RAW
#endif

#ifndef CONFIG_UDP_RECV_BUF_SIZE 
#define CONFIG_UDP_RECV_BUF_SIZE    1460
#endif


#ifdef CONFIG_SUPPORT_DYNAMIC_SYNC
typedef void (*dynamic_sync_recv_cb_t)(uint8_t* data,uint16_t len,uint32_t addr);
void register_dynamic_sync_udp_recv_callback(dynamic_sync_recv_cb_t cb);
void UDP_Send_sync(uint8_t *pdata,uint16_t len);
void UDP_Send_sync_unicast(uint8_t *pdata,uint16_t len,uint32_t addr);
#endif

void UDP_Send(char *pdata,uint16_t len);
void UDP_set_port(uint16_t port);
void udp_task_init();

#ifdef __cplusplus
}
#endif


#endif /*#ifndef __UDP_PERF_H__*/

