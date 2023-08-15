 
## release/v6,当前master
   - 支持蓝牙遥控转发
   - 新的log printf方式
   - 支持设置通电反应
   - 新的通用闹钟功能
   - 蓝牙配网增加查询uuid
   - HF-A11ASSISTHREAD增加查询uuid
   - 网络诊断
   - tcp udp 改成raw api
   - 蓝牙ota
   - 新ota命令（支持静默ota和成功后主动通知）
##  release/v5
   - feat/配网时云端配置增加“app”信息
   - feat/net type 4
   - feat/增加立即上报类型
   - fix/AT指令配置AP模式时，设置为网络类型3，关闭蓝牙
   - 取消上电读取LED模式0或0xff会强制设置出厂模式
   - 公版遥控
   - debug log支持查询最后十条控制命令
## release/v4
  - 支持蓝牙遥控
  - 蓝牙配网增加lver查询
  - feat/统一命令（功能查询）增加路由器信号值返回
  - fix/设置网络类型0的时候发送出厂指示灯事件
  - fix/云端拼包失败
  - 优化加密算法
  - feat/ap配网推送事件
  - fix/优化交互协议数据处理
  - feat/支持透传协议
  - 支持通用功能设定（遥控，指示灯）设定
## release/v3
