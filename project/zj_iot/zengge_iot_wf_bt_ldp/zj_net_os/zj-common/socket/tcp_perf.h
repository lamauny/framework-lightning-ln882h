

#ifndef __TCP_PERF_H__
#define __TCP_PERF_H__



#ifdef __cplusplus
extern "C" {
#endif

#define TCP_SEVER_PORT_DEFAULT 5577

#define TCPC_TIMEOUT   180

#ifdef CONFIG_USE_TCP_SERVER_SOCKET
#define USE_TCP_SERVER_SOCKET
#endif

#ifndef USE_TCP_SERVER_SOCKET
#define USE_TCP_SERVER_RAW
#endif

#ifndef CONFIG_TCPS_RECV_BUF_SIZE 
#define CONFIG_TCPS_RECV_BUF_SIZE    1460
#endif

void tcp_server_task_init();
void tcp_server_set_port(uint16_t port);
int  tcp_server_multi_send(unsigned char* buf,int len);
uint8_t tcp_server_get_client_amount();

#ifdef __cplusplus
}
#endif


#endif /*#ifndef __TCP_PERF_H__*/

