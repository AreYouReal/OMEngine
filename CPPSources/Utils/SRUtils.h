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

#define SRUTIL_API
#define SRCALLBACK

#define ES_WINDOW_RGB           0   /// SRCreateWindow flag - RGB color buffer
#define ES_WINDOW_ALPHA         1   /// SRCreateWindow flag - ALPHA color buffer
#define ES_WINDOW_DEPTH         2   /// SRCreateWindow flag - depth buffer
#define ES_WINDOW_STENCIL       4   /// SRCreateWindow flag - stencil buffer
#define ES_WINDOW_MULTISAMPLE   8   /// SRCreateWindow flat - multi-sample buffer

#pragma GENERAL
struct SRContext{
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
    void ( SRCALLBACK *drawFunc ) ( SRContext * );
    void ( SRCALLBACK *shutdownFunc ) ( SRContext * );
    void ( SRCALLBACK *keyFunc ) ( SRContext *, unsigned char, int, int );
    void ( SRCALLBACK *updateFunc ) ( SRContext *, float deltaTime );
};


#pragma Public Functions
GLboolean SRUTIL_API SRCreateWindow ( SRContext *context, const char *title, GLint width, GLint height, GLuint flags );

void SRUTIL_API SRRegisterDrawFunc ( SRContext *context, void ( SRCALLBACK *drawFunc ) ( SRContext * ) );

void SRUTIL_API SRRegisterShutdownFunc ( SRContext *context, void ( SRCALLBACK *shutdownFunc ) ( SRContext * ) );

void SRUTIL_API SRRegisterUpdateFunc ( SRContext *context, void ( SRCALLBACK *updateFunc ) ( SRContext *, float ) );

void SRUTIL_API SRRegisterKeyFunc ( SRContext *context,
                                   void ( SRCALLBACK *drawFunc ) ( SRContext *, unsigned char, int, int ) );



#pragma LOGGING
void logMessage(const char *formatString, ...);

#pragma mark FILE IO
srFile *fileOpen ( void *ioContext, const char *fileName );
long getFileSize( srFile *pFile );
void fileClose ( srFile *pFile );
int fileRead ( srFile *pFile, long bytesToRead, void *buffer );

std::shared_ptr<ShaderSource> readShaderFromFile( void *ioContext, const char *fileName);
void readOBJFromFile(void *ioContext, const char *fileName);