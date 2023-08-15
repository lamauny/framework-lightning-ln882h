#ifndef _NEW_CMD_H__
#define _NEW_CMD_H__

#include "new_data_event.h"
#include "new_dynamic.h"

#define NEW_CMD_UNIFIED_STATIC_SET       0x01
#define NEW_CMD_UNIFIED_DYNAMIC_SET      0x02
#define NEW_CMD_UNIFIED_CANDLE_SET       0x03
#define NEW_CMD_UNIFIED_CUSTOM_SET       0x04
#define NEW_CMD_UNIFIED_WAKEUP_SET       0x07
#define NEW_CMD_UNIFIED_WAKEUP_QUERY     0x08
#define NEW_CMD_UNIFIED_SLEEP_SET        0x09
#define NEW_CMD_UNIFIED_SLEEP_QUERY      0x0A
#define NEW_CMD_UNIFIED_COUNTDOWN_QUERY  0x0D
#define NEW_CMD_UNIFIED_EXIT_PREVIEW     0x0E
#define NEW_CMD_UNIFIED_MUSIC            0x0F

#define NEW_CMD_UNIFIED_MIC_SET          0x10
#define NEW_CMD_UNIFIED_MIC_COLOR_SET    0x11
#define NEW_CMD_UNIFIED_MIC_QUERY        0x12
#define NEW_CMD_UNIFIED_SORT_SET         0x13
#define NEW_CMD_UNIFIED_SORT_PREVIEW     0x15
#define NEW_CMD_UNIFIED_WIRING_SET       0x16

#endif