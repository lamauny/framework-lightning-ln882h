

INCLUDES += -I../../zj-common/app_general_funtion
INCLUDES += -I../../zj-common/btr
INCLUDES += -I../../zj-common/proto_dpc
INCLUDES += -I../../zj-common/rmt_scene
INCLUDES += -I../../zj-common/socket
INCLUDES += -I../../zj-common/sys
INCLUDES += -I../../zj-common/utils
INCLUDES += -I../../zj-common/zj_ota
INCLUDES += -I../../zj-common/net
INCLUDES += -I../../zj-common/lz4/lib
INCLUDES += -I../../zj-common/quicklz
INCLUDES += -I../../adapter_common
ifeq ($(CONFIG_SUPPORT_DYNAMIC_SYNC),1)
INCLUDES += -I../../zj-common/dynamic_sync
endif
