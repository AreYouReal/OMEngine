LOCAL_PATH			:= $(call my-dir)
SRC_PATH			:= $(LOCAL_PATH)/../../..

include $(CLEAR_VARS)

LOCAL_MODULE    := SRE
LOCAL_CFLAGS    += -std=c++11


LOCAL_SRC_FILES := $(LOCAL_PATH)/esUtil_Android.cpp \
				   $(SRC_PATH)/CPPSources/Game.cpp \
				   $(SRC_PATH)/CPPSources/Shaders/ShaderHelper.cpp \
				   $(SRC_PATH)/CPPSources/Shaders/ShaderProgram.cpp \
				   $(SRC_PATH)/CPPSources/Shaders/Shader.cpp \
				   $(SRC_PATH)/CPPSources/Utils/SRUtils.cpp \
				   $(SRC_PATH)/CPPSources/Utils/3dPart/loadpng/lodepng.cpp \
				   $(SRC_PATH)/CPPSources/Utils/3dPart/nvtristrip/NvTriStrip.cpp \
				   $(SRC_PATH)/CPPSources/Utils/3dPart/nvtristrip/NvTriStripObjects.cpp \
				   $(SRC_PATH)/CPPSources/Utils/3dPart/nvtristrip/VertexCache.cpp \
				   $(SRC_PATH)/CPPSources/Math/m4d.cpp \
				   $(SRC_PATH)/CPPSources/Math/v3d.cpp \
				   $(SRC_PATH)/CPPSources/Math/q4d.cpp \
				   $(SRC_PATH)/CPPSources/Utils/OBJ/obj.cpp \
				   $(SRC_PATH)/CPPSources/Utils/OBJ/ObjMesh.cpp \
				   $(SRC_PATH)/CPPSources/Utils/Texture.cpp

LOCAL_C_INCLUDES	:= $(SRC_PATH) \
					   $(COMMON_INC_PATH) \
					   $(SRC_PATH)/CPPSources \
					   $(SRC_PATH)/CPPSources/Shaders \
					   $(SRC_PATH)/CPPSources/Utils \
					   $(SRC_PATH)/CPPSources/Utils/3dPart/loadpng \
					   $(SRC_PATH)/CPPSources/Utils/3dPart/nvtristrip \
					   $(SRC_PATH)/CPPSources/Utils/OBJ \
					   $(SRC_PATH)/CPPSources/Math \
				   
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv3

LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
