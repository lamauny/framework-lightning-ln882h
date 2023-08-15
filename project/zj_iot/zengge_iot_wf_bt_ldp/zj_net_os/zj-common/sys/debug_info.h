/*
 * debug_info.h
 *
 *  Created on: 2019��2��15��
 *      Author: Administrator
 */

#ifndef APP_INCLUDE_DEBUG_INFO_H_
#define APP_INCLUDE_DEBUG_INFO_H_

#define INIT_ORIGIN_CODE            -9999
#define MAX_STORE_NUM               200000
#define ROUTER_RSSI_RECORD_PERIOD   60//sec

//#define CONFIG_DBG_TEST

#ifdef CONFIG_DBG_TEST
#define DBG_STORE_FAST_PERIOD       5 * 60//sec
#define DBG_STORE_SLOW_PERIOD       10 * 60//sec
#define DBG_INF_REPORT_PERIOD       30 * 60  //sec
#else
#define DBG_STORE_FAST_PERIOD       10 * 60//sec
#define DBG_STORE_SLOW_PERIOD       30 * 60//sec
#define DBG_INF_REPORT_PERIOD       12 * 60 * 60 //sec
#endif
typedef struct 
{
	int max;
	int min;
	int aver;
}value_family_t;

typedef enum{
    DBG_EVT_SERVER_CONNECTING,
	DBG_EVT_SERVER_CONNECTED,
	DBG_EVT_SERVER_DISCONNECTED,
	DBG_EVT_ROUTER_CONNECTING,
	DBG_EVT_ROUTER_CONNECTED,
	DBG_EVT_ROUTER_DISCONNECTED,  
	DBG_EVT_SERVER_CODE,
	DBG_EVT_DNS_CODE,
	DBG_EVT_HEARTBEAT_FAILED,
	DBG_EVT_HEARTBEAT_RESPONSE,
	DBG_EVT_WIFI_RESTART,
	DBG_EVT_STORE_TOTAL_RECORD,
	DBG_EVT_REST_FACTORY_SETTING,
	DBG_EVT_CFG_NET_ING,
	DBG_EVT_CFG_NET_SUCCESS,
	DBG_EVT_RESET_REASON,
	DBG_EVT_MIN_FREEHEAP,
	DBG_EVT_MAX_FREEHEAP,
	DBG_INF_STORE_NOW,
	DBG_INF_PRINTF,
	DBG_INF_SEND,
	DBG_AT_SEND
}dbg_evt_t;

void sys_dbg_init();
void sys_dbg_uuid_clear();
void sys_dbg_event(dbg_evt_t evt,void *data);
void sys_dbg_set_store_period(uint16_t time);
void sys_dbg_handle();
int sys_dbg_get_connect_code();
int sys_dbg_get_dns_code();
void sys_dbg_report_now();

#endif /* APP_INCLUDE_DEBUG_INFO_H_ */
