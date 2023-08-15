
## zj_net_os
### note : 编译前请检查更新对应分支、子模块
### 1、目录结构
```
    📦zj_net_os
    ┣ 📂adapter_common
    ┣ 📂sdk   
    ┗ 📂zj-common         
            
```
```
  zj_net_os
   ├───adapter_common                /* 链接sdk适配层 */
   ├───sdk                           /* 各类芯片厂商的sdk */
   │   ├───bk_sdk（git子模块）         /* 博通BL2028N的sdk */
   │   └───esp_sdk（git子模块）        /* ESP32的sdk适配组件 */
   │   └───bl_sdk（git子模块）         /* BL602的sdk */
   ├───zj-common                     /* 征极wifi iot设备通用组件 */
           ├───app_general_funtion       /* 应用通用功能 */
           ├───btr            /* 蓝牙遥控 */
           ├───dynamic_sync   /* 动态同步功能 */
           ├───net            /* wifi 处理 */
           ├───port           /* 处理sdk适配功能 */
           ├───proto_type     /* 通讯协议处理模块 */
           ├───lz4            /* lz4压缩算法 */
           ├───quicklz        /* Quicklz压缩算法 */
           ├───rmt_scene      /* 蓝牙遥控场景 */
           ├───socket         /* 网络socket:tcp server tcp client udp server ble */
           └───store_kv       /* flash：kv数据库(停用) */
           └───sys            /* 系统类事件处理 */
           └───utils          /* 工具类接口 */
           └───zj_ota         /* 固件升级 */
```
------------


### 2、组件依赖说明
```
  zj-common -> adapter_common->sdk
```
 - 框架依赖freeRTOS+LWIP
------------


