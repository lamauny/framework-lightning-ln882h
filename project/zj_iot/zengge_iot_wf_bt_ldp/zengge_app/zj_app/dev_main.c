#include "ZG_system.h"
#include "dev_common.h"
#include "version_config.h"

#define  TAG "dev_main"

const char IIC_CHIP_NAME_HEAD[4][5] = {{"BP1"},{"EGH1"},{"EH1"},{"EJ1"}};

static void fetch_bulid_time(char *time)
{

  char DateStr[20] = {0};
  char year[5] = {0},mon_str[4] = {0},day[3] = {0};
  char mon = 1;
  sprintf(DateStr,"%s",__DATE__);//Jul 23 2018  Aug  1 2019

  memcpy(mon_str,DateStr,3);
  memcpy(day,DateStr + 4,2);

  if(strchr(day,' ') != NULL){

     memcpy(day,"0",1);
  }
  memcpy(year,DateStr + 7,4);

  if (strcmp(mon_str, "Jan") == 0)  mon = 1;
  else if (strcmp(mon_str, "Feb") == 0)  mon = 2;
  else if (strcmp(mon_str, "Mar") == 0)  mon = 3;
  else if (strcmp(mon_str, "Apr") == 0)  mon = 4;
  else if (strcmp(mon_str, "May") == 0)  mon = 5;
  else if (strcmp(mon_str, "Jun") == 0)  mon = 6;
  else if (strcmp(mon_str, "Jul") == 0)  mon = 7;
  else if (strcmp(mon_str, "Aug") == 0)  mon = 8;
  else if (strcmp(mon_str, "Sep") == 0)  mon = 9;
  else if (strcmp(mon_str, "Oct") == 0)  mon = 10;
  else if (strcmp(mon_str, "Nov") == 0)  mon = 11;
  else mon = 12;


  sprintf(time,"%s%02d%s",year,mon,day);
  os_printf("\n\nbulid time %s\nlight version %s\n\n",time,APP_VERSION_STRING);
}

void ZG_light_system_start(void)
{
    char build_time[9] = {0};
    fetch_bulid_time(build_time);
    ZG_creat_device_id(MID_VERSION);/*配置MID:AK001-ZJ21xx*/
    ZG_device_info_conf(DEVICE_TYPE,PROTOCOL_VERSION,BLE_CHIP_ID);/*配置设备类型、应用协议大版本*/

#ifdef APPCONFIG_OUTPUT_DRV_IIC
    /* iic驱动电流配置 */
    #if (APPCONFIG_IC_TYPE == IIC_CHIP_BP)
    BP5758D_setElectricity_number(APPCONFIG_R_CUR,APPCONFIG_G_CUR,APPCONFIG_B_CUR,APPCONFIG_W_CUR,APPCONFIG_C_CUR);
    #elif (APPCONFIG_IC_TYPE == IIC_CHIP_EH)
    SM2135EH_setElectricity_number(APPCONFIG_R_CUR,APPCONFIG_W_CUR);
    #elif (APPCONFIG_IC_TYPE == IIC_CHIP_EJ)
    SM2135EJ_setElectricity_number(APPCONFIG_R_CUR,APPCONFIG_W_CUR);
    #elif (APPCONFIG_IC_TYPE == IIC_CHIP_EGH)
    SM2235Ex_setElectricity_number(APPCONFIG_R_CUR,APPCONFIG_W_CUR);
    #else
      #error "iic chip no choose!!!"
    #endif
    
#endif
#ifdef APP_CONFIG_RESET_METHOD2
   /* 恢复出厂选项2：如果没有配过网则物理开关四次恢复出厂设置，否则按照APPCONFIG_RESTORE_COUNT */
   if(is_once_cfg_net() == 0){

      zj_set_reset_factory_number(4);
   }else{
      zj_set_reset_factory_number(APPCONFIG_RESTORE_COUNT);
   }
#else
   zj_set_reset_factory_number(APPCONFIG_RESTORE_COUNT);
#endif
#if defined(APPCONFIG_OUTPUT_DRV_PWM) || defined(CUSTOMER_DIFF_IH9WL) || defined(CUSTOMER_DIFF_IH9W)
/* 
    配置固件版本号：
    FRIMWARE_VERSION：固件版本
    build_time：编译时间
    CUSTOMER_NAME：客户名称
    CHIP_MANUFACTURER：厂商标识
    VERSION_TYPE：固件标识 
*/
    ZG_device_version_conf(FRIMWARE_VERSION,build_time,CUSTOMER_NAME,CHIP_MANUFACTURER,VERSION_TYPE);  
#else 
    char g_iic_version_type[20] = {0};
    sprintf((char *)g_iic_version_type,"%s%02d",IIC_CHIP_NAME_HEAD[APPCONFIG_IC_TYPE],APPCONFIG_VER_TYPE);
    ZG_device_version_conf(FRIMWARE_VERSION,build_time,CUSTOMER_NAME,CHIP_MANUFACTURER,g_iic_version_type);
#endif 
    
    ZG_system_event_init();
    ZG_data_event_init();

    #ifdef APPCONFIG_OUTPUT_DRV_IIC
        zj_dbg_printf("iicI:chip%d:%d-%d-%d-%d-%d\r\n",
                        APPCONFIG_IC_TYPE,
                        APPCONFIG_R_CUR,
                        APPCONFIG_G_CUR,
                        APPCONFIG_B_CUR,
                        APPCONFIG_W_CUR,
                        APPCONFIG_W_CUR);
    #endif

    #ifndef USE_1M_PARTITION
    /*tcp sever overlap parser config*/
    pro_param_t param = {
              
        .head[0] = 0x31,
        .head[1] = 0x41,
        .head[2] = 0x42,
        .head[3] = 0x61,

        .head[4] = 0x36,
        .head[5] = 0x38,
        .head[6] = 0x3B,
        #ifdef APPCONFIG_MAGICHOME_LESS_5CH_CMD
        .body_len[0] = 8,
        .body_len[1] = 8,
        #else
        .body_len[0] = 9,
        .body_len[1] = 9,
        #endif
        .body_len[2] = 8,
        .body_len[3] = 5,
        .body_len[4] = 9,
        .body_len[5] = 5,
        .body_len[6] = 13,
        .number = 7
    };
    overlap_parser_set_config(&param);
    #endif
    ZG_storage_module_start();

    #ifdef APPCONFIG_SUPPORT_AC_MONITOR
    Restore_factory_settings_func(RESET_BY_POWER,SWITCH_THREE_TIMES_RESET_OFF);
    ac_monitor_init();
    #else
    Restore_factory_settings_func(RESET_BY_POWER,SWITCH_THREE_TIMES_RESET_ON);
    #endif
    

    power_on_set_status_default(APPCONFIG_DEF_PWRON_CFG);

    light_thread_init();  
    #ifdef APPCONFIG_SUPPORT_NIGHT_LAMP
    light_night_lamp_power_init();
    #endif
    light_remote_init();
    preview_task_init();
    ZG_monitor_start(10);
    #if defined(APPCONFIG_SUPPORT_KEY)
    light_key_init(); 
    #endif
    pwr_record_init();
    #ifdef CONFIG_SUPPORT_DYNAMIC_SYNC
    light_dynamic_sync_init();
    #endif
}
