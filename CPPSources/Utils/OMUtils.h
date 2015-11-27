#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <memory>
#include <cmath>

#include "lodepng.h"

#include "FileContent.h"
#include "Stopwatch.h"
#include "m4d.h"        // ALL MATH
#include "Shortcuts.h"

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
using srFile = AAsset;
#else
using srFile = FILE;
#endif

#define ES_WINDOW_RGB           0   /// SRCreateWindow flag - RGB color buffer
#define ES_WINDOW_ALPHA         1   /// SRCreateWindow flag - ALPHA color buffer
#define ES_WINDOW_DEPTH         2   /// SRCreateWindow flag - depth buffer
#define ES_WINDOW_STENCIL       4   /// SRCreateWindow flag - stencil buffer
#define ES_WINDOW_MULTISAMPLE   8   /// SRCreateWindow flat - multi-sample buffer

enum class TouchState{ BEGIN, MOVED, CANCELLED, ENDED };

template<typename T>
using sp = std::shared_ptr<T>;

struct OMContext;
using UpdateFunc    = void(*)(OMContext*, const float);
using DrawFunc      = void(*)(OMContext*);
using TouchFunc     = void(*)(OMContext*, const int, const int, const int);
using ShutDownFunc  = void(*)(OMContext*);


#pragma GENERAL
struct OMContext{
    void       *platformData;    /// Put platform specific data here
    void       *userData;        /// Put your user data here...
    GLint       width;           /// Window width
    GLint       height;          /// Window height
    
#ifndef __APPLE__
    EGLNativeDisplayType eglNativeDisplay;   /// Display handle
    EGLNativeWindowType  eglNativeWindow;    /// Window handle
    EGLDisplay  eglDisplay;                  /// EGL display
    EGLContext  eglContext;                  /// EGL context
    EGLSurface  eglSurface;                  /// EGL surface
#endif
    
    /// Callbacks
    UpdateFunc      updateFunc;
    DrawFunc        drawFunc;
    TouchFunc       touchFunc;
    ShutDownFunc    shutdownFunc;
};



#pragma Public Functions
GLboolean SRCreateWindow ( OMContext *context, const char *title, GLint width, GLint height, GLuint flags );

void SRRegisterDrawFunc ( OMContext *context, DrawFunc );

void SRRegisterShutdownFunc ( OMContext *context, ShutDownFunc );

void SRRegisterUpdateFunc ( OMContext *context, UpdateFunc );

void SRRegisterTouchFunc ( OMContext *context, TouchFunc );



#pragma LOGGING
void logMessage(const char *formatString, ...);
void printGLString(string name, GLenum s);


#pragma mark FILE IO
srFile *fileOpen ( void *ioContext, const char *fileName );
long getFileSize( srFile *pFile );
void fileClose ( srFile *pFile );
long fileRead ( srFile *pFile, long bytesToRead, void *buffer );

std::unique_ptr<FileContent> readTextFile( void *ioContext, std::string fileName);
std::unique_ptr<FileContent> readBytesFromFile(void *ioContext, const char *fileName);
std::vector<unsigned char> loadRawPNGData(void *ioContext, const char *filename, unsigned int &width, unsigned int &height);
