#ifndef UTILS_ZLOG_H
#define UTILS_ZLOG_H

#include "zj_adapt_api.h"

#define OS_LOGD(start, fmt, ...)  os_printf("\033[36;1m""D(%s) (%ld) "fmt"\n""\e[0m", start,(uint32_t)PORT_CONFIG_SYSTEM_TIME,##__VA_ARGS__) 
#define OS_LOGW(start, fmt, ...)  os_printf("\033[33;1m""W(%s) "fmt"\n""\e[0m", start,##__VA_ARGS__) 	
#define OS_LOGI(start, fmt, ...)  os_printf("\033[36;1m""I(%s) "fmt"\n""\e[0m" ,start, ##__VA_ARGS__) 
#define OS_LOGE(start, fmt, ...)  os_printf("\033[31;1m""E(%s) "fmt"\n""\e[0m",start,  ##__VA_ARGS__)

#define APP_LOGD(start, fmt, ...) zj_printf("\033[37m""D(APP:%s) (%ld) "fmt"\n""\e[0m", start,(uint32_t)PORT_CONFIG_SYSTEM_TIME,##__VA_ARGS__)
#define APP_LOGW(start, fmt, ...) zj_printf("\033[33m""W(APP:%s) "fmt"\n""\e[0m",start, ##__VA_ARGS__)
#define APP_LOGI(start, fmt, ...) zj_printf("\033[37m""I(APP:%s) "fmt"\n""\e[0m",start, ##__VA_ARGS__)
#define APP_LOGE(start, fmt, ...) zj_printf("\033[35m""E(APP:%s) "fmt"\n""\e[0m",start, ##__VA_ARGS__)

#define MESH_LOGD(start, fmt, ...) os_printf("\033[36;7m""D(%s) (%ld) "fmt"\n""\e[0m", start,(uint32_t)PORT_CONFIG_SYSTEM_TIME,##__VA_ARGS__)
#define MESH_LOGW(start, fmt, ...) os_printf("\033[33;7m""W(%s) "fmt"\n""\e[0m",start, ##__VA_ARGS__)
#define MESH_LOGI(start, fmt, ...) os_printf("\033[36;7m""I(%s) "fmt"\n""\e[0m",start, ##__VA_ARGS__)
#define MESH_LOGE(start, fmt, ...) os_printf("\033[31;7m""E(%s) "fmt"\n""\e[0m",start, ##__VA_ARGS__)
    
#endif


