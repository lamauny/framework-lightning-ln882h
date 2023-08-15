set(srcs    "app_general_funtion/new_clock.c"
            "app_general_funtion/pwr_on_setting.c"
            "btr/bt_remote.c"
            "btr/btr.c"
            "btr/pub_btr.c"
            "net/bridge_ble_central_evt.c"
            "net/net_utils.c"
            "net/WIFI_config.c"
            "proto_dpc/proto_data_event.c"
            "proto_dpc/proto_parser_pack.c"
            "rmt_scene/rmt_scene_list.c"
            "rmt_scene/rmt_scene.c"
            "socket/ble_os_handle.c"
            "socket/ble_payload.c"
            "socket/ble_transfer.c"
            "socket/tcp_client.c"
            "socket/tcp_perf.c"
            "socket/udp_perf.c"
            "socket/zj_http_client.c"
            "socket/tcp_test.c"
            "socket/zj_ping.c"
            "sys/AT_CMD.c"
            "sys/debuginfo.c"
            "sys/encryption.c"
            "sys/net_diagnosis.c"
            "sys/ZG_system_entry.c"
            "sys/product_test.c"
            "sys/default_cfg_evt.c"
            "utils/color.c"
            "utils/json_char_convert.c"
            "utils/report_type.c"
            "utils/timeStamp.c"
            "utils/utils_comp.c"
            "utils/utils_json.c"
            "utils/utils_linked_list.c"
            "utils/utils_tcplist.c"
            "utils/ZG_utils.c"
            "utils/zJSON.c"
            "zj_ota/fota.c"
            "zj_ota/zjos_ota.c"
            "port/zj_port.c"
            "quicklz/quicklz.c"
            "quicklz/zj_quicklz.c"
            "lz4/lib/lz4.c")

if(CONFIG_SUPPORT_DYNAMIC_SYNC)
    list(APPEND srcs
        "dynamic_sync/sync_hbm.c"
        "dynamic_sync/sync_mdls.c")
endif()

set(includes   "zj_ota" 
                "utils"
                "sys"
                "socket"
                "rmt_scene"
                "proto_dpc"
                "net"
                "btr"
                "app_general_funtion"
                "quicklz"
                "dynamic_sync"
                "lz4/lib")
set(requires "zj_adapt")

idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS ${includes}
                       PRIV_REQUIRES adapter_common zj_adapt esp_common)
