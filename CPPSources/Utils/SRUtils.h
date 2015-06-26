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

#include "zlib/zlib.h"
#include "zlib/unzip.h"
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
void fileClose ( srFile *pFile );
int fileRead ( srFile *pFile, int bytesToRead, void *buffer );

void adjustFilePath(char *filepath);
void getFilePath(char* filepath, char *path);
void getFileName(char *filepath, char *filename);
void getFileExtension(char *filepath, char *ext, bool uppercase);

Memory *mopen( char *filename, bool relative_path );
Memory *mclose( Memory *memory );
unsigned int mread( Memory *memory, void *dst, unsigned int size );
void minsert(Memory *memory, char *str, unsigned int position );

std::shared_ptr<ShaderSource> readShaderFromFile( void *ioContext, const char *fileName);