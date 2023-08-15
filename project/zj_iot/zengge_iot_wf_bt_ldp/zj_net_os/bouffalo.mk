# Component Makefile
#
include $(BL60X_SDK_PATH)/components/network/ble/ble_common.mk
## These include paths would be exported to project level
COMPONENT_ADD_INCLUDEDIRS += ./ \
                              zj-common \
                              zj-common/net \
                              zj-common/socket \
                              zj-common/proto_dpc \
                              zj-common/store_kv/include\
                              zj-common/sys \
                              zj-common/utils\
                              zj-common/btr\
                              zj-common/app_general_funtion\
                              zj-common/zj_ota\
                              zj-common/rmt_scene\
                              adapter_common\
                              zj-common/dynamic_sync\
                              zj-common/quicklz\
                              zj-common/lz4/lib\

## not be exported to project level
COMPONENT_PRIV_INCLUDEDIRS := ./ \
                              zj-common \
                              zj-common/net \
                              zj-common/socket \
                              zj-common/proto_dpc \
                              zj-common/store_kv/include\
                              zj-common/sys \
                              zj-common/utils\
                              zj-common/btr\
                              zj-common/app_general_funtion\
                              zj-common/zj_ota\
                              adapter_common\
                              zj-common/dynamic_sync\
                              zj-common/quicklz\
                              zj-common/lz4/lib\

## This component's src
COMPONENT_SRCS := $(wildcard zj-common/net/*.c) \
                  $(wildcard zj-common/store_kv/*.c) \
                  $(wildcard zj-common/proto_dpc/*.c) \
                  $(wildcard zj-common/socket/*.c) \
                  $(wildcard zj-common/sys/*.c) \
                  $(wildcard zj-common/utils/*.c) \
                  $(wildcard zj-common/quicklz/*.c) \
                  $(wildcard zj-common/lz4/lib/*.c) \
                  $(wildcard zj-common/btr/*.c) \
                  $(wildcard zj-common/app_general_funtion/*.c) \
                  $(wildcard zj-common/zj_ota/*.c) \
                  $(wildcard zj-common/rmt_scene/*.c) \
                  $(wildcard zj-common/port/*.c) \
                  $(wildcard adapter_common/*.c) \
                  $(wildcard zj-common/dynamic_sync/*.c) \
                  $(wildcard ./*.c) \
                  $(filter-out ${OUT_SRC}, $(wildcard example/*.c))

COMPONENT_OBJS := $(patsubst %.c,%.o, $(COMPONENT_SRCS))
COMPONENT_OBJS := $(patsubst %.cpp,%.o, $(COMPONENT_OBJS))

COMPONENT_SRCDIRS := zj-common/utils \
                     zj-common/net \
                     zj-common/port \
                     zj-common/proto_dpc \
                     zj-common/socket \
                     zj-common/store_kv \
                     zj-common/quicklz\
                     zj-common/lz4/lib\
                     zj-common/sys \
                     zj-common/app_general_funtion\
                     zj-common/btr\
                     zj-common/zj_ota\
                     zj-common/rmt_scene\
                     zj-common/port\
                     adapter_common\
                     zj-common/dynamic_sync\
                     ./
                 
COMPONENT_LIB_ONLY := 1
LIBS ?= zj_net_os
COMPONENT_ADD_LDFLAGS += -L$(COMPONENT_PATH)/lib $(addprefix -l,$(LIBS))
ALL_LIB_FILES := $(patsubst %,$(COMPONENT_PATH)/lib/lib%.a,$(LIBS))
COMPONENT_ADD_LINKER_DEPS := $(ALL_LIB_FILES)