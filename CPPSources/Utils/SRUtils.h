#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#else
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

#ifdef ANDROID
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/asset_manager.h>
typedef AAsset srFile;
#else
typedef FILE srFile;
#endif

void logMessage(const char *formatString, ...);