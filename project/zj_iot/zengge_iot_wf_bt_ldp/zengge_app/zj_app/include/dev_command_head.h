#ifndef __HEAD_EVENT_TYPE_H__
#define __HEAD_EVENT_TYPE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define COMMAND_TIME_SET         0x10
#define COMMAND_TIME_QUERY       0x11
#define COMMAND_CLOCK_SET        0x21
#define COMMAND_CLOCK_QUERY      0x22
#define COMMAND_COLOR_SET        0x31
#define COMMAND_CCT_SET          0x35
#define COMMAND_UNIFIED_MUSIC_SET        0x36
#define COMMAND_UNIFIED_MUSIC_CONFIG     0x37
#define COMMAND_UNIFIED_DYNAMIC_SET      0x38
#define COMMAND_UNIFIED_CANDLE_SET       0x39
#define COMMAND_UNIFIED_BLINK_SET        0x3A
#define COMMAND_UNIFIED_STATIC_SET       0x3B
#define COMMAND_MUSIC_SET_FAST   0x41
#define COMMAND_MUSIC_SET_SLOW   0x42
#define COMMAND_USERDEFINE_SET   0x51
#define COMMAND_USERDEFINE_QUERY 0x52
#define COMMAND_DYNAMIC_SET      0x61
#define COMMAND_SET_SORT         0x62
#define COMMAND_TEST_MODE        0x63
#define COMMAND_SWTICH           0x71
#define COMMAND_MIC_QUERY        0x72
#define COMMAND_MIC_SET          0x73
#define COMMAND_MIC_COLOR_SET    0x74
#define COMMAND_MIC_QUERY_VER2   0x75
#define COMMAND_DEVICE_QUERY     0x81
#define COMMAD_REPORT_IMMEDIATELY 0x2C


#ifdef __cplusplus
}
#endif


#endif 
