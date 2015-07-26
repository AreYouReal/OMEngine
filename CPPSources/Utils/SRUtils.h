#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <vector>

#include "lodepng.h"

#include "ShaderSource.h"
#include "esUtil.h"

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

#define MAX_PATH 2048

struct Memory{
    char            filename[MAX_PATH];
    unsigned int    size;
    unsigned int    position;
    unsigned char   *buffer;
};

void logMessage(const char *formatString, ...);

srFile *fileOpen ( void *ioContext, const char *fileName );
long getFileSize( srFile *pFile );
void fileClose ( srFile *pFile );
int fileRead ( srFile *pFile, long bytesToRead, void *buffer );

std::shared_ptr<ShaderSource> readShaderFromFile( void *ioContext, const char *fileName);
void readOBJFromFile(void *ioContext, const char *fileName);