#ifndef _OS_STORE_H__
#define _OS_STORE_H__

#include "zj_adapt_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define KEY_ENC1_STORE                    "KEN1"
#define KEY_ENC2_STORE                    "KEN2"

#if defined(USE_1M_PARTITION) || defined(IS_BK_PLATFORM)

#define KEY_HOST_STORE                    "KHOS"
#define KEY_PORT_STORE                    "KPOR"
#define KEY_WIFI_MODE_STORE               "KWMO"
#define KEY_WIFI_SSID_STORE               "KSID"
#define KEY_WIFI_PWD_STORE                "KPWD"
#define KEY_CLOUD_SWITCH_STORE            "KCST"
#define KEY_RESTORE_STORE                 "KRES"
#define KEY_ST1_STORE                     "KST1"
#define KEY_ST2_STORE                     "KST2"
#define KEY_PRINTF_EN_STORE               "KPEN"
#define KEY_UUID_STORE                    "KUID"
#define KEY_AES_STORE                     "KAES"
#define KEY_LED_MODE_STORE                "KMOD"
#define KEY_CONFIGNET_FLAG_STORE          "KNET"
#define KEY_REMOTE_ENCRY_TABLE_STORE      "KRET"
#define KEY_REMOTE_ENCRY_VER_STORE        "KREV"
#define KEY_BTR_ADDR_STORE                "KRAR"
#define KEY_BTR_SCENE_STORE               "KBTR"
#define KEY_BTR_SCENE2_STORE              "K2BR"
#define KEY_BTR_SCENE3_STORE              "K3BR"
#define KEY_BTR_SCENE4_STORE              "K4BR"
#define KEY_ERROR_LOG_STORE               "KERR"
#define KEY_BRT_CFG                       "KCFG"
#define KEY_DF_POWER_SET                  "KDPS"
#define KEY_APP_VER_STORE                 "KAPV"
#define KEY_PUBTR_STORE                   "KPTR"
#define KEY_PWRCFG                        "KPCF"
#define KEY_PWR_TWO_FORCE_PWR_ON          "KPTF"
#define KEY_FOTAOLDVER_STORE              "KOTA"
#define KEY_FOTADSTVER_STORE              "KOTD"
#define KEY_FOTAIND_STORE                 "KIND"
#else
#define KEY_HOST_STORE                   "KEY_HOST"
#define KEY_PORT_STORE                   "KEY_WIFI_PORT"
#define KEY_WIFI_MODE_STORE              "KEY_WIFI_MODE"
#define KEY_WIFI_SSID_STORE              "KEY_WIFI_SSID"
#define KEY_WIFI_PWD_STORE               "KEY_WIFI_PWD"
#define KEY_CLOUD_SWITCH_STORE           "KEY_CLOUD"
#define KEY_RESTORE_STORE                "KEY_RESTORE"
#define KEY_ST1_STORE                    "KEY_ST1"
#define KEY_ST2_STORE                    "KEY_ST2"
#define KEY_PRINTF_EN_STORE              "KEY_PRINTF_EN"
#define KEY_UUID_STORE                   "KEY_UUID"
#define KEY_AES_STORE                    "KEY_AES"
#define KEY_LED_MODE_STORE               "KEY_LED_MODE"
#define KEY_CONFIGNET_FLAG_STORE         "KEY_CFGNET_FG"
#define KEY_REMOTE_ENCRY_TABLE_STORE      "KEY_REET"
#define KEY_REMOTE_ENCRY_VER_STORE        "KEY_REEV"
#define KEY_BTR_ADDR_STORE                "KEY_RADR"
#define KEY_BTR_SCENE_STORE               "KEY_BTRSS"
#define KEY_BTR_SCENE2_STORE              "KEY_2BTRSS"
#define KEY_BTR_SCENE3_STORE              "KEY_3BTRSS"
#define KEY_BTR_SCENE4_STORE              "KEY_4BTRSS"
#define KEY_ERROR_LOG_STORE               "KEY_ERROR"
#define KEY_BRT_CFG                       "KCFG"
#define KEY_DF_POWER_SET                  "KDPS"
#define KEY_APP_VER_STORE                 "KAV"
#define KEY_PUBTR_STORE                   "KPBTR"
#define KEY_PWRCFG                        "KPCF"
#define KEY_PWR_TWO_FORCE_PWR_ON          "KPTF"
#define KEY_FOTAOLDVER_STORE              "KFOTA"
#define KEY_FOTADSTVER_STORE              "KFOTD"
#define KEY_FOTAIND_STORE                 "KUPIND"
#endif
#define KEY_TCPCONNONCE_STORE             "KTCS"
#define KEY_RST_METHOD2                   "KRM2"
#define KEY_DBG_STORE                     "KSDE"
#define KEY_DBG_ADD_STORE                 "KSDA"
#define KEY_ICON_STORE                    "KICO"

#ifdef __cplusplus
}
#endif


#endif /*#ifndef __TCP_PERF_H__*/

