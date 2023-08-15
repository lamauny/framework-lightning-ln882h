#ifndef KV_CONFIG_H
#define KV_CONFIG_H

#include "zj-adapt/zj_adapt_config.h"

#if USE_ZG_KV_MODULE_OPTION

#define KV_MODULE_STR_HDR_LENGTH    2
#define KV_MODULE_INT_LENGTH        strlen("65536")
#define KV_MODULE_KEY_LENGTH        KV_MODULE_STR_HDR_LENGTH + KV_MODULE_INT_LENGTH + 1

#endif

#endif

