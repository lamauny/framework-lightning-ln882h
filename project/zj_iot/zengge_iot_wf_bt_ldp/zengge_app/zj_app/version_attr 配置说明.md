#### 配置说明
------------
 -  PROTOCOL_VERSION：应用版本协议版本号
 -  APPCONFIG_SUPPORT_IR：支持IR功能                                   
 -  APPCONFIG_PWM_DEPTH：配置PWM频率
 -  APPCONFIG_DEFF_MODE ：配置烧录出厂后的模式
 -  APPCONFIG_REST_MODE  :恢复出厂的模式
 -  APPCONFIG_DEF_PWRON_CFG ： 默认出厂通电反应设置
 -  APPCONFIG_SUPPORT_COLOR_WARM ：支持暖白功能
 - APPCONFIG_SUPPORT_COLOR_RGB：支持RGB功能
 - APPCONFIG_SUPPORT_COLOR_CCT：支持CCT功能
 -  APPCONFIG_DEF_WIRING_TYPE ：配置默认接线类型设置
 -  APPCONFIG_RESTORE_COUNT ：配置上电多少次恢复出厂设置 
 -  APPCONFIG_LONG_PRESS_RESTORE_TIME ： 配置长按按键恢复出厂设置时间设置 
 -  APPCONFIG_MAGICHOME_LESS_5CH_CMD  ：支持 magic home：设备类型<5路的兼容 
 -  APPCONFIG_SUPPORT_PUB_BTR： 支持公版遥控 
 -  APPCONFIG_SUPPORT_PUB_BTR_TYPE_H2：  支持支持公版H2遥控 
 -  APPCONFIG_NONSUPPORT_BR_DIM_CURVE：  支持取消亮度调光曲线 
 -  APPCONFIG_NONSUPPORT_LOW_LIGHT_LEVEL： 支持取消低亮度处理 
 -  APPCONFIG_RESET_CLR_CFG  ：支持恢复出厂清除参数 
 - APPCONFIG_SUPPORT_KEY：支持按键功能
 - APPCONFIG_SUPPORT_KEY_LPD：支持按键长按类型：长按调光
 -  APPCONFIG_SUPPORT_HW_MIC：支持硬件麦克风功能
 - APPCONFIG_SUPPORT_PRODUCT_TEST：支持产测功能
 - APPCONFIG_ENABLE_CFG_RGBWC_BOTH：支持RGB和CCT同时亮功能
------------
#### 枚举说明
------------
##### PWM输出通道排序：
 -  APPCONFIG_OUTPUT_SORT_CH1     ： 默认通道1颜色输出排序   
 -  APPCONFIG_OUTPUT_SORT_CH2     ： 默认通道2颜色输出排序   
 -  APPCONFIG_OUTPUT_SORT_CH3     ：默认通道3颜色输出排序   
 -  APPCONFIG_OUTPUT_SORT_CH4     ： 默认通道4颜色输出排序   
 -  APPCONFIG_OUTPUT_SORT_CH5     ： 默认通道5颜色输出排序   

------------


##### 灯光输出类型：
 -  APPCONFIG_OUTPUT_DRV_PWM    ： 灯光输出类型为PWM
 - APPCONFIG_OUTPUT_DRV_IIC：灯光输出类型为IIC
 ------------
##### 测试、老化、恢复模式定义 
- DEF_FACMODE_OTHER_MODE ： 其他出厂模式
- DEF_FACMODE_RGB_JUMP       ：RGB跳变
- DEF_FACMODE_RGBW_JUMP    ：RGBW跳变
- DEF_FACMODE_RGBWC_JUMP  ：RGBWC跳变
- DEF_FACMODE_RGBWC_TEST   ：RGBWC测试模式
- DEF_FACMODE_CCT_TEST         ：CCT测试模式
- DEF_FACMODE_WARM_TEST    ：暖白测试模式
- DEF_RESET_RGBWC_AGING      ：RGBWC老化模式
- DEF_RESET_CCT_AGING             ：CCT老化模式
- DEF_RESET_WARM_AGING       ：暖白老化模式

------------
 ##### 默认上电状态定义
 - PWR_ON_STATUS_DEFAULT：默认暖白
 - PWR_ON_STATUS_MEMORY：记忆状态
 -  PWR_ON_STATUS_CUSTOM：自定义颜色
 -  PWR_ON_STATUS_ALWAY_ON_MEMORY：总是开启并记忆状态
 -  PWR_ON_STATUS_ALWAY_OFF：总是关闭

------------
 ##### 接线类型定义
- WIRING_TYPE_NONE          ： 不支持
- WIRING_TYPE_RGB             ： RGB接线类型
- WIRING_TYPE_RGBW          ：RGBW不同时亮接线类型
- WIRING_TYPE_RGBW_BOTH  ：RGBW同时亮接线类型
- WIRING_TYPE_RGBWC         ：RGBWC不同时亮接线类型
- WIRING_TYPE_RGBWC_BOTH  ：RGBWC同时亮接线类型
- WIRING_TYPE_W            ： 暖白接线类型
- WIRING_TYPE_C            ： 冷白接线类型
- WIRING_TYPE_WC         ： 色温接线类型

