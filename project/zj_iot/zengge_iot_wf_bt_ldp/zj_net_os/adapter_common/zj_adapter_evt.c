#include "zj_adapt_api.h"
#include <stddef.h>
#include "string.h"
#include "stdio.h"

static zj_adapter_func_t s_event_handler_cb;
static uint8_t g_dev_name[22];
static uint8_t g_raw_adv_data[31];
static uint8_t g_raw_scan_rsp_data[31];
static uint8_t g_adv_update_enable,g_relay_enable;

static void __ble_update_condition_get_cb(void *param)
{
   uint8_t *data = (uint8_t *)param;
   g_adv_update_enable = *data;
}

static void __btr_relay_condition_get_cb(void *param)
{
   uint8_t *data = (uint8_t *)param;
   g_relay_enable = *data;    
}

static void __ble_adv_data_get_cb(void *param)
{
    uint8_t *adv_data = (uint8_t *)param;
    memcpy(g_raw_adv_data,adv_data,BLE_ADV_DATA_LENGTH);
}

static void __ble_scanResp_get_cb(void *param)
{
    uint8_t *data = (uint8_t *)param;
    memcpy(g_raw_scan_rsp_data,data,31);
}

static void __ble_name_get_cb(void *param)
{
    uint8_t *data = (uint8_t *)param;
    memset(g_dev_name,0,22);
    memcpy(g_dev_name,data,22);  
}


void zj_adapter_post_event(zj_adapter_evt_t evt,void *data1,void *data2,int len)
{
    if(s_event_handler_cb.adapter_evt_cb_t){

       (*s_event_handler_cb.adapter_evt_cb_t)(evt,data1,data2,len);
    }
}

uint8_t *zj_adapter_get_adv_data_config()
{
   if(s_event_handler_cb.adapter_ble_advData_get){

     s_event_handler_cb.adapter_ble_advData_get(__ble_adv_data_get_cb);
     return g_raw_adv_data;
   }
   return NULL;
}


uint8_t *zj_adapter_get_adv_scanResp_config()
{
   if(s_event_handler_cb.adapter_ble_ScanResp_get){

     s_event_handler_cb.adapter_ble_ScanResp_get(__ble_scanResp_get_cb);
     return g_raw_scan_rsp_data;
   }
   return NULL;
}

uint8_t *zj_adapter_get_adv_name_config()
{
   if(s_event_handler_cb.adapter_ble_name_get){

     s_event_handler_cb.adapter_ble_name_get(__ble_name_get_cb);
   }
   return g_dev_name;
}

uint8_t zj_adapter_post_adv_update_condition_get()
{
   if(s_event_handler_cb.adapter_ble_update_condition_get){

      s_event_handler_cb.adapter_ble_update_condition_get(__ble_update_condition_get_cb);
   }
   return g_adv_update_enable;
}

uint8_t zj_adapter_post_btr_realy_condition_get()
{
   if(s_event_handler_cb.adapter_btr_relay_condition_get){

      s_event_handler_cb.adapter_btr_relay_condition_get(__btr_relay_condition_get_cb);
   }
   return g_relay_enable;
}

void register_zj_adapter_callback(zj_adapter_func_t cb)
{
   s_event_handler_cb = cb;
} 
