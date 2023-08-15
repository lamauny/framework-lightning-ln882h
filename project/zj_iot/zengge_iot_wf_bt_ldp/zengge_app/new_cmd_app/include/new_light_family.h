#ifndef _NEW_FAMILY__H
#define _NEW_FAMILY__H

#include "light_main.h"
#include "light_dynamic.h"
#include "new_dynamic.h"

#include "version_config.h"

typedef struct
{
  light_family_t led_family;
  candle_conf_t candle;
  uint8_t new_custom_cmd[109];
  #ifdef APPCONFIG_MAGICHOME_CMD
  uint8_t old_custom[66];
  #endif
}light_para_assembly_t;

#define KEY_NEW_FALLASLEEP_STORE       "NKFSS"
#define KEY_NEW_WAKEUP_STORE           "NKWUS"
#define KEY_NEW_CUSTOM_STORE           "NKCUSS"
#define KEY_NEW_CUSTOM_NUM_STORE       "NKCUSNM"

#endif