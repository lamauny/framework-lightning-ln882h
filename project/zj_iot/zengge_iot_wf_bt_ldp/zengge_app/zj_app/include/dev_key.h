#ifndef DEV_KEY_H
#define DEV_KEY_H

#define KEY_NEW_SCENE_STORE              "LNS"

#ifdef USE_1M_PARTITION

#define KEY_COLOR_STORE                  "LCR"
#define KEY_CCT_STORE                    "LCC"
#define KEY_LED_SPEED_STORE              "LSP"
#define KEY_LED_CUSTOM_MODE_STORE        "LCU"
#define KEY_LED_SORT_STORE               "LSO"
#define KEY_CLOCK_STORE                  "LCK"
#define KEY_LED_SWITCH_STORE             "LPW"
#define KEY_CANDLE_STORE                 "LCA"
#define KEY_RGB_BRIGHTNESS_STORE         "LRB"
#define KEY_CCT_BRIGHTNESS_STORE         "LCB"
#define KEY_RUNNING_MODE_STORE           "LRM"
#define KEY_MIC_SW_STORE                 "LMP"
#define KEY_MIC_SENS_STORE               "LMS"
#define KEY_MUSIC_TYPE_STORE             "LMT"
#define KEY_MUSIC_COLOR_STORE            "LMC"
#define KEY_REMOTE_DIY_STORE             "LRD"
#define KEY_REMOTE_AUTO_STORE            "LRA"
#define KEY_COLOR_FLAG                   "LCF"
#define KEY_PWR_FULL_FLAG                "LPF"
#define KEY_PWR_MSG_ID                   "LPM"
#define KEY_WIRING_TYPE                  "LWR"
#define KEY_SCENE_LIST                   "LEL"
#define KEY_SCENE_POS                    "LEP"

#else

#define KEY_COLOR_STORE                  "KEY_COLOR"
#define KEY_CCT_STORE                    "KEY_CCT"
#define KEY_LED_SPEED_STORE              "KEY_SPEED"
#define KEY_LED_CUSTOM_MODE_STORE        "KEY_CUSTOM_MODE"
#define KEY_LED_SORT_STORE               "KEY_SORT"
#define KEY_CLOCK_STORE                  "KEY_CLOCK"
#define KEY_LED_SWITCH_STORE             "KEY_LED_SWITCH"
#define KEY_CANDLE_STORE                 "KEY_CANDLE"
#define KEY_RGB_BRIGHTNESS_STORE         "NKRGBR"
#define KEY_CCT_BRIGHTNESS_STORE         "NKCCTR"
#define KEY_RUNNING_MODE_STORE           "NKRNM"
#define KEY_MIC_SW_STORE                 "KEY_MSW"
#define KEY_MIC_SENS_STORE               "KEY_MSS"
#define KEY_MUSIC_TYPE_STORE             "KEY_MT"
#define KEY_MUSIC_COLOR_STORE            "KEY_MSC"
#define KEY_REMOTE_DIY_STORE             "KEY_RDS"
#define KEY_REMOTE_AUTO_STORE            "KEY_RAS"
#define KEY_COLOR_FLAG                   "NCFL"
#define KEY_PWR_FULL_FLAG                "NPLFL"
#define KEY_PWR_MSG_ID                   "NPLID"
#define KEY_WIRING_TYPE                  "NWRTY"

#endif

#endif