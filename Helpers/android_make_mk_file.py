import os, sys

android_mk_prefix = "LOCAL_PATH			:= $(call my-dir)\nSRC_PATH			:= $(LOCAL_PATH)/../../..\n\ninclude $(CLEAR_VARS)\n\nLOCAL_MODULE    := SRE\nLOCAL_CFLAGS    += -std=c++11"
android_mk_postfix = "LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv3\n\nLOCAL_STATIC_LIBRARIES := android_native_app_glue\n\ninclude $(BUILD_SHARED_LIBRARY)\n\n$(call import-module,android/native_app_glue)"

android_mk_source_prefix = "LOCAL_SRC_FILES := $(LOCAL_PATH)/esUtil_Android.cpp \\\n"
android_mk_include_prefix = "LOCAL_C_INCLUDES	:= $(SRC_PATH) \\\n $(COMMON_INC_PATH) \\\n"

android_src_local_path = "$(SRC_PATH)"

source_files = []
header_dirs = []

sourcePath = "../CPPSources"

android_mk_path = "../Platforms/Android/jni/Android.mk"

def fill_data(dir):
	global source_files
	global header_dirs
	for name in os.listdir(dir):
		path = os.path.join(dir, name)
		if os.path.isfile(path):
			filename, ext = os.path.splitext(path);
			if ext == ".cpp":
				source_files.append(android_src_local_path + path[2:]);
			else:
				if ext == ".h" or ext == ".hpp":
					toAppend = android_src_local_path + os.path.dirname(path)[2:]
					if toAppend not in header_dirs:
						header_dirs.append(toAppend)
		else:
			fill_data(path);


def write_android_mk(path):
	global source_files
	global header_dirs
	file = open(path, "w");

	srcContent = android_mk_source_prefix;
	for source in source_files:
		srcContent += source + "\\\n"

	srcContent += "\n\n"

	srcContent += android_mk_include_prefix;

	for headerDir in header_dirs:
		srcContent += headerDir + "\\\n"

	file.write(android_mk_prefix + "\n\n");

	file.write(srcContent + "\n\n");

	file.write(android_mk_postfix);

	file.close()


fill_data(sourcePath);
write_android_mk(android_mk_path);


