
#include "zj_adapt_api.h"
#include "zj_adapt_config.h"
#include <stdbool.h>
/* BLE */
// #include "nimble/nimble_port.h"
// #include "nimble/nimble_port_freertos.h"
// #include "host/ble_hs.h"
// #include "host/util/util.h"
// #include "console/console.h"
// #include "services/gap/ble_svc_gap.h"
// #include "host/ble_uuid.h"
// #include "services/gatt/ble_svc_gatt.h"
// #include "services/ans/ble_svc_ans.h"
// #include "esp_log.h"

#define ADV_SCAN_UNIT(_ms) ((_ms) * 8 / 5)
#define SCAN_INTERVAL_MS 70
#define SCAN_WINDOW_MS   20
#define SCAN_INTERVAL    ADV_SCAN_UNIT(SCAN_INTERVAL_MS)
#define SCAN_WINDOW      ADV_SCAN_UNIT(SCAN_WINDOW_MS)

#define    TAG  "BTADPTR"
static int __gap_event(struct ble_gap_event *event, void *arg);
static void __device_adv();
static uint8_t own_addr_type;
static uint16_t g_conn_handle;
static uint16_t g_mtu;
static uint8_t g_is_peripheral_connected;
static uint16_t g_notify_handle;
static bool g_is_bt_scan_enable;
static bool g_ble_init_flag;
/**
 * The vendor specific security test service consists of two characteristics:
 *     o random-number-generator: generates a random 32-bit number each time
 *       it is read.
 *     o static-value: a single-byte characteristic that can always be read.
 */

// static const ble_uuid16_t gatt_svr_svc_light   = BLE_UUID16_INIT(0xFFFF);
// static const ble_uuid16_t gatt_svr_light_chr_w = BLE_UUID16_INIT(0xff01);
// static const ble_uuid16_t gatt_svr_light_chr_r = BLE_UUID16_INIT(0xff02);

// static const ble_uuid16_t gatt_svr_svc_ota   = BLE_UUID16_INIT(0xFE00);
// static const ble_uuid16_t gatt_svr_ota_chr_w = BLE_UUID16_INIT(0xff11);
// static const ble_uuid16_t gatt_svr_ota_chr_r = BLE_UUID16_INIT(0xff22);

static uint8_t gatt_svr_light_static_val[256];

// static int
// gatt_svr_light_access_cb(uint16_t conn_handle, uint16_t attr_handle,
//                              struct ble_gatt_access_ctxt *ctxt,
//                              void *arg);

// static const struct ble_gatt_svc_def gatt_svr_svcs[] = {
//     {
//         /*** Service: Security test. */
//         .type = BLE_GATT_SVC_TYPE_PRIMARY,
//         .uuid = &gatt_svr_svc_light.u,
//         .characteristics = (struct ble_gatt_chr_def[])
//         { {
//                 /*** Characteristic: Random number generator. */
//                 .uuid = &gatt_svr_light_chr_w.u,
//                 .access_cb = gatt_svr_light_access_cb,
//                 .flags = BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_WRITE_NO_RSP
//             }, {
//                 /*** Characteristic: Static value. */
//                 .uuid = &gatt_svr_light_chr_r.u,
//                 .access_cb = gatt_svr_light_access_cb,
//                 .flags = BLE_GATT_CHR_F_NOTIFY,
//                 .val_handle = &g_notify_handle,
//             }, {
//                 0, /* No more characteristics in this service. */
//             }
//         },
//     },

//     {
//         /*** Service: Security test. */
//         .type = BLE_GATT_SVC_TYPE_PRIMARY,
//         .uuid = &gatt_svr_svc_ota.u,
//         .characteristics = (struct ble_gatt_chr_def[])
//         { {
//                 /*** Characteristic: Random number generator. */
//                 .uuid = &gatt_svr_ota_chr_w.u,
//                 .access_cb = gatt_svr_light_access_cb,
//                 .flags = BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_WRITE_NO_RSP
//             }, {
//                 /*** Characteristic: Static value. */
//                 .uuid = &gatt_svr_ota_chr_r.u,
//                 .access_cb = gatt_svr_light_access_cb,
//                 .flags = BLE_GATT_CHR_F_NOTIFY
//             }, {
//                 0, /* No more characteristics in this service. */
//             }
//         },
//     },
//     {

//     }
// };

// static int
// gatt_svr_light_access_cb(uint16_t conn_handle, uint16_t attr_handle,
//                              struct ble_gatt_access_ctxt *ctxt,
//                              void *arg)
// {
//     const ble_uuid_t *uuid;
//     int rc;

//     uuid = ctxt->chr->uuid;

//     /* Determine which characteristic is being accessed by examining its
//      * 128-bit UUID.
//      */
//     if (ble_uuid_cmp(uuid, &gatt_svr_light_chr_w.u) == 0) {
//         switch (ctxt->op) {
//         case BLE_GATT_ACCESS_OP_READ_CHR:

//             return 0;

//         case BLE_GATT_ACCESS_OP_WRITE_CHR:{
                                   
//             uint8_t om_len = OS_MBUF_PKTLEN(ctxt->om);
//             ble_hs_mbuf_to_flat(ctxt->om, gatt_svr_light_static_val, 256, NULL);
//             zj_adapter_post_event(ADAPT_EVT_BLE_LIGHT_DATA,gatt_svr_light_static_val,NULL,om_len);                     
//         }return rc;

//         default:
//             assert(0);
//         }
           
//     }
//     else if (ble_uuid_cmp(uuid, &gatt_svr_ota_chr_w.u) == 0) {
 
//     }
//     /* Unknown characteristic; the nimble stack should not have called this
//      * function.
//      */
//     assert(0);
//     return BLE_ATT_ERR_UNLIKELY;
// }

// void
// gatt_svr_register_cb(struct ble_gatt_register_ctxt *ctxt, void *arg)
// {
//     char buf[BLE_UUID_STR_LEN];

//     switch (ctxt->op) {
//     case BLE_GATT_REGISTER_OP_SVC:
//         ESP_LOGI(TAG, "registered service %s with handle=%d\n",
//                     ble_uuid_to_str(ctxt->svc.svc_def->uuid, buf),
//                     ctxt->svc.handle);
//         break;

//     case BLE_GATT_REGISTER_OP_CHR:
//         ESP_LOGI(TAG, "registering characteristic %s with "
//                     "def_handle=%d val_handle=%d\n",
//                     ble_uuid_to_str(ctxt->chr.chr_def->uuid, buf),
//                     ctxt->chr.def_handle,
//                     ctxt->chr.val_handle);
//         break;

//     case BLE_GATT_REGISTER_OP_DSC:
//         ESP_LOGI(TAG, "registering descriptor %s with handle=%d\n",
//                     ble_uuid_to_str(ctxt->dsc.dsc_def->uuid, buf),
//                     ctxt->dsc.handle);
//         break;

//     default:
//         assert(0);
//         break;
//     }
// }

int
gatt_svr_init(void)
{
    int rc;

    // ble_svc_gap_init();
    // ble_svc_gatt_init();
    // ble_svc_ans_init();

    // rc = ble_gatts_count_cfg(gatt_svr_svcs);
    // if (rc != 0) {
    //     return rc;
    // }

    // rc = ble_gatts_add_svcs(gatt_svr_svcs);
    // if (rc != 0) {
    //     return rc;
    // }

    return 0;
}


void ble_store_config_init(void);
/**
 * Logs information about a connection to the console.
 */
// static void
// bleprph_print_conn_desc(struct ble_gap_conn_desc *desc)
// {
//     ESP_LOGI(TAG, "handle=%d our_ota_addr_type=%d our_ota_addr=%02x:%02x:%02x:%02x:%02x:%02x",
//              desc->conn_handle, desc->our_ota_addr.type,
//              desc->our_ota_addr.val[5],
//              desc->our_ota_addr.val[4],
//              desc->our_ota_addr.val[3],
//              desc->our_ota_addr.val[2],
//              desc->our_ota_addr.val[1],
//              desc->our_ota_addr.val[0]);

//     ESP_LOGI(TAG, "our_id_addr_type=%d our_id_addr=%02x:%02x:%02x:%02x:%02x:%02x",
//              desc->our_id_addr.type,
//              desc->our_id_addr.val[5],
//              desc->our_id_addr.val[4],
//              desc->our_id_addr.val[3],
//              desc->our_id_addr.val[2],
//              desc->our_id_addr.val[1],
//              desc->our_id_addr.val[0]);

//     ESP_LOGI(TAG, "peer_ota_addr_type=%d peer_ota_addr=%02x:%02x:%02x:%02x:%02x:%02x",
//              desc->peer_ota_addr.type,
//              desc->peer_ota_addr.val[5],
//              desc->peer_ota_addr.val[4],
//              desc->peer_ota_addr.val[3],
//              desc->peer_ota_addr.val[2],
//              desc->peer_ota_addr.val[1],
//              desc->peer_ota_addr.val[0]);

//     ESP_LOGI(TAG, "peer_id_addr_type=%d peer_id_addr=%02x:%02x:%02x:%02x:%02x:%02x",
//              desc->peer_id_addr.type,
//              desc->peer_id_addr.val[5],
//              desc->peer_id_addr.val[4],
//              desc->peer_id_addr.val[3],
//              desc->peer_id_addr.val[2],
//              desc->peer_id_addr.val[1],
//              desc->peer_id_addr.val[0]);

//     ESP_LOGI(TAG, "conn_itvl=%d conn_latency=%d supervision_timeout=%d "
//                 "encrypted=%d authenticated=%d bonded=%d",
//                 desc->conn_itvl, desc->conn_latency,
//                 desc->supervision_timeout,
//                 desc->sec_state.encrypted,
//                 desc->sec_state.authenticated,
//                 desc->sec_state.bonded);
// }

/**
 * The nimble host executes this callback when a GAP event occurs.  The
 * application associates a GAP event callback with each connection that forms.
 * bleprph uses the same callback for all connections.
 *
 * @param event                 The type of event being signalled.
 * @param ctxt                  Various information pertaining to the event.
 * @param arg                   Application-specified argument; unused by
 *                                  bleprph.
 *
 * @return                      0 if the application successfully handled the
 *                                  event; nonzero on failure.  The semantics
 *                                  of the return code is specific to the
 *                                  particular GAP event being signalled.
 */
// static int
// __gap_event(struct ble_gap_event *event, void *arg)
// {
//     struct ble_gap_conn_desc desc;
//     int rc;

//     switch (event->type) {
//     case BLE_GAP_EVENT_DISC:                           
//         if(event->disc.length_data > 28 && event->disc.length_data <= 31){
                            
//             uint8_t adv_data[32 + 6] = {0};
        
//             memcpy(adv_data,event->disc.addr.val, 6);
//             memcpy(adv_data + 6,event->disc.data,event->disc.length_data);
//             zj_adapter_post_event(ADAPT_EVT_BLE_RMT,adv_data,(int *)&event->disc.rssi,event->disc.length_data + 6);
//         }
//         return 0;
//     case BLE_GAP_EVENT_CONNECT:
//         /* A new connection was established or a connection attempt failed. */
//         ESP_LOGI(TAG, "connection %s; status=%d ",
//                     event->connect.status == 0 ? "established" : "failed",
//                     event->connect.status);
//         if (event->connect.status == 0) {
//             rc = ble_gap_conn_find(event->connect.conn_handle, &desc);
//             assert(rc == 0);
//             bleprph_print_conn_desc(&desc);
//         }

//         if (event->connect.status != 0) {
//             /* Connection failed; resume advertising. */
//             __device_adv();
//         }else{
//             zj_adapter_post_event(ADAPT_EVT_BLE_CONNECTED,NULL,NULL,0);
//             g_is_peripheral_connected = true;
//             g_conn_handle = event->connect.conn_handle;
//         }
//         return 0;

//     case BLE_GAP_EVENT_DISCONNECT:
//         ESP_LOGI(TAG, "disconnect; reason=%d ", event->disconnect.reason);
//         bleprph_print_conn_desc(&event->disconnect.conn);

//         /* Connection terminated; resume advertising. */
//         __device_adv();
//         zj_adapter_post_event(ADAPT_EVT_BLE_DISCONNECTED,NULL,NULL,0);
//         g_is_peripheral_connected = false;
//         return 0;

//     case BLE_GAP_EVENT_CONN_UPDATE:
//         /* The central has updated the connection parameters. */
//         ESP_LOGI(TAG, "connection updated; status=%d ",
//                     event->conn_update.status);
//         rc = ble_gap_conn_find(event->conn_update.conn_handle, &desc);
//         assert(rc == 0);
//         bleprph_print_conn_desc(&desc);
//         return 0;

//     case BLE_GAP_EVENT_ADV_COMPLETE:
//         ESP_LOGI(TAG, "advertise complete; reason=%d",
//                     event->adv_complete.reason);
//       //  __device_adv();
//         return 0;

//     case BLE_GAP_EVENT_ENC_CHANGE:
//         /* Encryption has been enabled or disabled for this connection. */
//         ESP_LOGI(TAG, "encryption change event; status=%d ",
//                     event->enc_change.status);
//         rc = ble_gap_conn_find(event->enc_change.conn_handle, &desc);
//         assert(rc == 0);
//         bleprph_print_conn_desc(&desc);
//         return 0;

//     case BLE_GAP_EVENT_SUBSCRIBE:
//         ESP_LOGI(TAG, "subscribe event; conn_handle=%d attr_handle=%d "
//                     "reason=%d prevn=%d curn=%d previ=%d curi=%d",
//                     event->subscribe.conn_handle,
//                     event->subscribe.attr_handle,
//                     event->subscribe.reason,
//                     event->subscribe.prev_notify,
//                     event->subscribe.cur_notify,
//                     event->subscribe.prev_indicate,
//                     event->subscribe.cur_indicate);
//         return 0;

//     case BLE_GAP_EVENT_MTU:
//         ESP_LOGI(TAG, "mtu update event; conn_handle=%d cid=%d mtu=%d",
//                     event->mtu.conn_handle,
//                     event->mtu.channel_id,
//                     event->mtu.value);
//         g_mtu = event->mtu.value;
//         return 0;

//     case BLE_GAP_EVENT_REPEAT_PAIRING:
//         /* We already have a bond with the peer, but it is attempting to
//          * establish a new secure link.  This app sacrifices security for
//          * convenience: just throw away the old bond and accept the new link.
//          */

//         /* Delete the old bond. */
//         rc = ble_gap_conn_find(event->repeat_pairing.conn_handle, &desc);
//         assert(rc == 0);
//         ble_store_util_delete_peer(&desc.peer_id_addr);

//         /* Return BLE_GAP_REPEAT_PAIRING_RETRY to indicate that the host should
//          * continue with the pairing operation.
//          */
//         return BLE_GAP_REPEAT_PAIRING_RETRY;
//     }

//     return 0;
// }

static void
bleprph_on_reset(int reason)
{
    // ESP_LOGE(TAG, "Resetting state; reason=%d", reason);
}

static void
bleprph_on_sync(void)
{
    // int rc;

    // rc = ble_hs_util_ensure_addr(0);
    // assert(rc == 0);

    // /* Figure out address to use while advertising (no privacy for now) */
    // rc = ble_hs_id_infer_auto(0, &own_addr_type);
    // if (rc != 0) {
    //     ESP_LOGE(TAG, "error determining address type; rc=%d", rc);
    //     return;
    // }

    // /* Printing ADDR */
    // uint8_t addr_val[6] = {0};
    // rc = ble_hs_id_copy_addr(own_addr_type, addr_val, NULL);

    // ESP_LOGI(TAG, "Device Address:%02x:%02x:%02x:%02x:%02x:%02x",
    //          addr_val[5],
    //          addr_val[4],
    //          addr_val[3],
    //          addr_val[2],
    //          addr_val[1],
    //          addr_val[0]);
}

void bleprph_host_task(void *param)
{
    // ESP_LOGI(TAG, "BLE Host Task Started");
    // /* This function will return only when nimble_port_stop() is executed */
    // nimble_port_run();

    // nimble_port_freertos_deinit();
    // ESP_LOGW(TAG, "nimble_port_freertos_deinit");
}



void zj_ble_scan_stop()
{  
//    if(g_is_bt_scan_enable == true){

//        ble_gap_disc_cancel();
//        ESP_LOGI(TAG,"zj_ble_scan_stop\n\n");
//    }
}


void zj_ble_scan_start()
{
//    if(g_is_bt_scan_enable == false){


//         uint8_t own_addr_type;
//         struct ble_gap_disc_params disc_params;
//         int rc;

//         /* Figure out address to use while advertising (no privacy for now) */
//         rc = ble_hs_id_infer_auto(0, &own_addr_type);
//         if (rc != 0) {
//             MODLOG_DFLT(ERROR, "error determining address type; rc=%d\n", rc);
//             return;
//         }
//         /* Tell the controller to filter duplicates; we don't want to process
//             * repeated advertisements from the same device.
//             */
//         disc_params.filter_duplicates = 0;

//         /**
//          * Perform a passive scan.  I.e., don't send follow-up scan requests to
//          * each advertiser.
//          */
//         disc_params.passive = 1;

//         /* Use defaults for the rest of the parameters. */
//         disc_params.itvl = SCAN_INTERVAL_MS;
//         disc_params.window = SCAN_WINDOW_MS;
//         disc_params.filter_policy = 0;
//         disc_params.limited = 0;
        
//         rc = ble_gap_disc(own_addr_type, BLE_HS_FOREVER, &disc_params,
//                             __gap_event, NULL);
//         if (rc != 0) {
//             MODLOG_DFLT(ERROR, "Error initiating GAP discovery procedure; rc=%d own_addr_type %d\n",
//                         rc,own_addr_type);
//         }
//    }
//    ESP_LOGW(TAG,"zj_ble_scan_start %d\n\n",g_is_bt_scan_enable);
}

void zj_btr_adv_payload_update(uint8_t *p)
{

}

// struct ble_hs_adv_fields adv_fields;
// struct ble_hs_adv_fields scan_fields;
static void __device_adv()
{	
    // struct ble_gap_adv_params adv_params;
    // int rc = ble_gap_adv_set_data(zj_adapter_get_adv_data_config(),BLE_ADV_DATA_LENGTH);
    // ESP_LOGI(TAG,"ble_gap_adv_set_data err = %d",rc);
    // rc = ble_gap_adv_rsp_set_data(zj_adapter_get_adv_scanResp_config(),31);
    // ESP_LOGI(TAG,"ble_gap_adv_rsp_set_data err = %d",rc);
    // /* Begin advertising. */
    // memset(&adv_params, 0, sizeof adv_params);
    // adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
    // adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;
    // rc = ble_gap_adv_start(own_addr_type, NULL, BLE_HS_FOREVER,
    //                        &adv_params, __gap_event, NULL);
    // ESP_LOGW(TAG, "device_adv rc=%d\n\n", rc);
    // if (rc != 0) {
    //     ESP_LOGE(TAG, "error enabling advertisement; rc=%d", rc);
    //     return;
    // }
}

static void __btr_forward_adv()
{
 
}

void zj_ble_adv_update()
{
//    if(zj_adapter_post_adv_update_condition_get() && g_ble_init_flag){

//       ble_gap_adv_stop();;
//       vTaskDelay(200 / portTICK_RATE_MS);
//        __device_adv();
//     }
}

uint8_t zj_ble_get_connected_status()
{
    return g_is_peripheral_connected ;
}

void zj_ble_notify(uint8_t *data,int len)
{
    // struct os_mbuf *om;
    // om = ble_hs_mbuf_from_flat(data, len);
    // if (om == NULL) {
    //     /* Memory not available for mbuf */
    //     ESP_LOGE(TAG, "No MBUFs available from pool, retry..");
    //     vTaskDelay(100 / portTICK_PERIOD_MS);
    //     om = ble_hs_mbuf_from_flat(data, len);
    //     //assert(om != NULL);
    // }
    // int rc = 0;
    // rc = ble_gattc_notify_custom(g_conn_handle, g_notify_handle, om);
    //assert(rc == 0);
}

void zj_ble_ota_notify(uint8_t *data,int len)
{
  
}

uint16_t zj_ble_get_mtu()
{
    if(g_mtu > 255){

        g_mtu = 255;
    }
	return g_mtu;
}


void zj_ble_adv_start()
{
//   if(g_ble_init_flag && !g_is_peripheral_connected){

//     __device_adv();
//   }
}

void zj_ble_adv_stop()
{
//    ble_gap_adv_stop();
}

void zj_ble_drv_deinit()
{
    // if(!g_ble_init_flag){

    //     return;
    // }
    // g_ble_init_flag = false;
    // nimble_port_stop();
    // nimble_port_deinit();

    // #ifdef CONFIG_WIFI_BEST_PERFORMANCE_OPTION
    // vTaskDelay(100);
    // esp_wifi_set_ps(WIFI_PS_NONE);
    // ESP_LOGI(TAG,"WIFI_PS_NONE");
    // #endif
    // zj_adapter_post_event(ADAPT_EVT_BLE_DEINIT_DONE,NULL,NULL,0); 
}

/*******************************************************/
void zj_ble_drv_init()
{
    // if(g_ble_init_flag){

    //     return;
    // }
    // #ifdef CONFIG_WIFI_BEST_PERFORMANCE_OPTION
    // esp_wifi_set_ps(WIFI_PS_MIN_MODEM);
    // ESP_LOGI(TAG,"WIFI_PS_MIN_MODEM");
    // vTaskDelay(50);
    // #endif
    // g_ble_init_flag = true;
    // nimble_port_init();
    // /* Initialize the NimBLE host configuration. */
    // ble_hs_cfg.reset_cb = bleprph_on_reset;
    // ble_hs_cfg.sync_cb = bleprph_on_sync;
    // ble_hs_cfg.gatts_register_cb = gatt_svr_register_cb;
    // ble_hs_cfg.store_status_cb = ble_store_util_status_rr;

    // int rc = gatt_svr_init();
    // assert(rc == 0);

    // /* Set the default device name. */
    // rc = ble_svc_gap_device_name_set((char *)zj_adapter_get_adv_name_config());
    // assert(rc == 0);

    // /* XXX Need to have template for store */
    // ble_store_config_init();

    // nimble_port_freertos_init(bleprph_host_task);
    // zj_adapter_post_event(ADAPT_EVT_BLE_INIT_DONE,NULL,NULL,0); 
}


