#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <vector>

#include "lodepng.h"

#include "ShaderSource.h"


#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#include "FileWrapper.h"
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

std::shared_ptr<ShaderSource> readShaderFromFile( void *ioContext, const char *fileName);