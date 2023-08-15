

#ifndef __PTEST_H__
#define __PTEST_H__

typedef enum{
   
   PT_EVT_INIT_DONE,
   PT_EVT_RSSI_BAD,
   PT_EVT_SCAN_FAIL,
   PT_EVT_SUCCESS,
}pt_evt_t;

#define  PT_MODE_NOMAL_SSID_HEAD    "zengge_npt_"
#define  PT_MODE_FAST_SSID_HEAD     "zengge_fpt_"
#define  PT_MODE_DETAIL_SSID_HEAD   "zengge_dpt_"
#define  PT_MODE_SCAN_SSID_HEAD     "zengge_spt_"
#define  PT_MODE_SCAN_SSID2_HEAD    "tuya_mdev_test1"

typedef void (*ptest_event_cb_t)(pt_evt_t evt);

void ptest_init();
void register_ptest_event(ptest_event_cb_t cb);
void ptest_start();
void ptest_stop();
void ptest_scan_post(char *ssid,int rssi);

#endif 

