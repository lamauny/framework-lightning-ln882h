
#ifndef BT_REMOTE_H
#define BT_REMOTE_H

#define OPCODE_BIND             0x01
#define OPCODE_LIGHT_SET        0x3B

#define GROUP_ALL_CONTROL       0x00

#define BTR_BIND_ADR_MAX_NUM    16
#define BRT_BIND_TIMEOUT        10 // sec

#ifndef CONFIG_BTR_RCV_TYPE_MAX_NUM     
#define CONFIG_BTR_RCV_TYPE_MAX_NUM  10
#endif

typedef enum 
{
	BTR_EVENT_BIND_ADDR,
	BTR_EVENT_UNBIND_ADDR,
	BTR_EVENT_CONTROL,
	BTR_EVENT_SCENE_RECORD,
	BTR_EVENT_SCENE_TRIGGER
}btr_event_t;

typedef enum{
   
   RECV_ALL_REMOTE,
   RECV_BY_CHOICE
}recv_type_choost_t;

typedef enum{

  BLE_CID_TYPE_CONTROLLER   = 0x71,
  BLE_CID_TYPE_ILLUMINATION = 0x72,
  BLE_CID_TYPE_ALL          = 0x00
}btr_cid_type_t;

typedef struct
{
    btr_event_t event;
    uint8_t relay_id;       /* 转发id */
    uint8_t type;           /* 遥控类型 */
    uint8_t protocol;       /* 协议版本号 */
    uint8_t addr[3];        /* 遥控地址 */
    uint8_t key;            /* 按键key值 */
    uint8_t para[10];       /* 通用命令参数 */
    uint8_t sn;
    uint8_t group_id;       /* 分组id */
    uint8_t scene_id;       /* 0~15,共16种场景 */ 
    bool bind_timeout_flag; /* 绑定超时标志位 */
}btr_arg_t;

typedef enum{
  
  REMOTE_CFG_BAN = 0x01,      /* 禁止使用遥控 */
  REMOTE_CFG_CURRENCY, /* 所有遥控可以控制  */
  REMOTE_CFG_BIND      /* 遥控需要绑定 */
}btr_cfg_t;

struct target_sn_handle_t
{
    uint8_t mac[3];
    int sn;
    uint32_t tick;
};

#define TARGET_SN_HANDLE_MAX_NUM         8

typedef int (*btr_event_cb_t)(btr_arg_t *arg);

void ble_remote_data_input(uint8_t *data);
 /**
 * @brief 遥控初始化
 * @note 
 * @param[in]  none
 * @param[out] none
 *
 * @return none
 */
void bt_remote_init();
/**
 * @brief 注册遥控回调事件并创建遥控系统task
 * @note 
 * @param[in]  btr_cb 标准遥控回调事件
 * @param[in]  pub_cb 公版遥控回调事件
 *
 * @return none
 */
int bt_remote_event_init(btr_event_cb_t btr_cb,btr_event_cb_t pub_cb);
/**
 * @brief 注册遥控透传数据回调事件
 * @note 
 * @param[in]  btr_cb 标准遥控数据透传回调
 * @param[in]  pub_cb 公版遥控数据透传回调
 *
 * @return none
 */
int bt_remote_tt_cb_init(btr_event_cb_t btr_cb,btr_event_cb_t pub_cb);
/**
 * @brief 设置遥控配置
 * @note  
 * @param[in] btr_cfg_t
 * @param[out] none
 *
 * @return none
 */
void bt_remote_set_config(btr_cfg_t cfg);
btr_cfg_t bt_remote_get_config();
/**
 * @brief 出厂默认遥控配置
 * @note  
 * @param[in] btr_cfg_t
 * @param[out] none
 *
 * @return none
 */
void bt_remote_set_default_config(btr_cfg_t cfg);
/**
 * @brief 设置绑定超时标志
 * @note  
 * @param[in] flag 1 : timeout 
 * @param[out] none
 *
 * @return none
 */
void bt_remote_set_binding_timeout_flag(bool flag);
bool bt_remote_get_binding_timeout_flag();
/**
 * @brief 设置超时时间（秒）
 * @note  
 * @param[in] time:sec
 * @param[out] none
 *
 * @return none
 */
void bt_remote_bind_timeout_sec_set(uint32_t time);
uint32_t bt_remote_get_bind_timeout_sec();
/**
 * @brief 删除所有遥控绑定
 * @note  
 * @param[in] none
 * @param[out] none
 *
 * @return none
 */
void bt_remote_delete_all_binding();
uint8_t bt_remote_get_bind_count();
/**
 * @brief 重新开放绑定
 * @note  时间为：bt_remote_get_bind_timeout_sec()
 * @param[in] none
 * @param[out] none
 *
 * @return none
 */
void bt_remote_open_binding();
/**
 * @brief 获取遥控初始时间
 * @note  
 * @param[in] none
 * @param[out] none
 *
 * @return 初始时间
 */
uint32_t bt_remote_get_init_time();

void bt_remote_scene_save(uint8_t *data,uint8_t id,int single_id_size);
/**
 * @brief 设置上电读取多少个场景
 * @note  不设置默认为16
 * @param[in] num : only:4 | 8 | 12 | 16
 * @param[out] none
 *
 * @return 0: success  -1 : false
 */
int bt_remote_set_scene_flash_read_number(uint8_t num);
/**
 * @brief 场景读取
 * @note 
 * @param[in] data : 16个场景数据
 * @param[in] single_id_size : 单个场景大小
 * @param[out] none
 *
 * @return none
 */
void bt_remote_scene_flash_read(uint8_t *data,int single_id_size);
  /**
 * @brief 查询是否是测试广播内容
 * @note 
 * @param[in] src : 广播内容
 * @param[out] src : option : 蓝牙关闭选项 
 *  - 0xA0 :FACTORY_TEST_SHUTDOWN_BLE_ADV
 *  - 0xA1 : FACTORY_TEST_SHUTDOWN_BLE_ADV_WITH_SCAN
 *
 * @return 0 : success else fail
 */
int query_whether_test_adv(uint8_t *src,uint8_t *option);

bool btr_sn_handler(uint8_t *mac,uint8_t sn);

#endif