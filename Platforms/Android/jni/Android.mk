LOCAL_PATH			:= $(call my-dir)
SRC_PATH			:= $(LOCAL_PATH)/../../..
COMMON_PATH			:= $(SRC_PATH)/Common
COMMON_INC_PATH		:= $(COMMON_PATH)/Include
COMMON_SRC_PATH		:= $(COMMON_PATH)/Source

include $(CLEAR_VARS)

LOCAL_MODULE    := SRE
LOCAL_CFLAGS    += -std=c++11


LOCAL_SRC_FILES := $(COMMON_SRC_PATH)/esShader.cpp \
				   $(COMMON_SRC_PATH)/esShapes.cpp \
				   $(COMMON_SRC_PATH)/esTransform.cpp \
				   $(COMMON_SRC_PATH)/esUtil.cpp \
				   $(COMMON_SRC_PATH)/Android/esUtil_Android.cpp \
				   $(SRC_PATH)/CPPSources/Main.cpp \
				   
				   
				   

LOCAL_C_INCLUDES	:= $(SRC_PATH) \
					   $(COMMON_INC_PATH) \
					   $(SRC_PATH)/CPPSources
				   
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv3

LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
