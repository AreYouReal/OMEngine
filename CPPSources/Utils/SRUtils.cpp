
#ifdef __APPLE__
#include "FileWrapper.h"
#endif

#include "SRUtils.h"


#ifndef __APPLE__
#pragma pack(push,x1)                            // Byte alignment (8-bit)
#pragma pack(1)
#endif

#ifndef __APPLE__
#pragma pack(pop,x1)
#endif

#ifndef __APPLE__

/** Check whether EGL_KHR_create_context extension is supported.  If so,
return EGL_OPENGL_ES3_BIT_KHR instead of EGL_OPENGL_ES2_BIT */
EGLint GetContextRenderableType ( EGLDisplay eglDisplay ){
#ifdef EGL_KHR_create_context
    const char *extensions = eglQueryString ( eglDisplay, EGL_EXTENSIONS );
    // check whether EGL_KHR_create_context is in the extension string
    if ( extensions != NULL && strstr( extensions, "EGL_KHR_create_context" ) ){
        return EGL_OPENGL_ES3_BIT_KHR; // extension is supported
    }
#endif
    return EGL_OPENGL_ES2_BIT;          // extension is not supported
}
#endif


#pragma GENERAL

GLboolean SRUTIL_API SRCreateWindow ( SRContext *context, const char *title, GLint width, GLint height, GLuint flags ){
#ifndef __APPLE__
    EGLConfig config;
    EGLint majorVersion;
    EGLint minorVersion;
    EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
    
    if ( context == NULL ){
        return GL_FALSE;
    }
    
#ifdef ANDROID
    // For Android, get the width/height from the window rather than what the
    // application requested.
    context->width = ANativeWindow_getWidth ( context->eglNativeWindow );
    context->height = ANativeWindow_getHeight ( context->eglNativeWindow );
#else
    context->width = width;
    Context->height = height;
#endif
    
    context->eglDisplay = eglGetDisplay( context->eglNativeDisplay );
    if ( context->eglDisplay == EGL_NO_DISPLAY ){
        return GL_FALSE;
    }
    
    // Initialize EGL
    if ( !eglInitialize ( context->eglDisplay, &majorVersion, &minorVersion ) ){
        return GL_FALSE;
    }
    
    {
        EGLint numConfigs = 0;
        EGLint attribList[] = {
            EGL_RED_SIZE,       5,
            EGL_GREEN_SIZE,     6,
            EGL_BLUE_SIZE,      5,
            EGL_ALPHA_SIZE,     8,
            EGL_DEPTH_SIZE,     8,
            EGL_STENCIL_SIZE,   8,
            EGL_SAMPLE_BUFFERS, ( flags & ES_WINDOW_MULTISAMPLE ) ? 1 : 0,
            // if EGL_KHR_create_context extension is supported, then we will use
            // EGL_OPENGL_ES3_BIT_KHR instead of EGL_OPENGL_ES2_BIT in the attribute list
            EGL_RENDERABLE_TYPE, GetContextRenderableType ( context->eglDisplay ),
            EGL_NONE
        };
        
        // Choose config
        if ( !eglChooseConfig ( context->eglDisplay, attribList, &config, 1, &numConfigs ) ){
            return GL_FALSE;
        }
        
        if ( numConfigs < 1 ){
            return GL_FALSE;
        }
    }
    
    
#ifdef ANDROID
    // For Android, need to get the EGL_NATIVE_VISUAL_ID and set it using ANativeWindow_setBuffersGeometry
    {
        EGLint format = 0;
        eglGetConfigAttrib ( context->eglDisplay, config, EGL_NATIVE_VISUAL_ID, &format );
        ANativeWindow_setBuffersGeometry ( context->eglNativeWindow, 0, 0, format );
    }
#endif // ANDROID
    // Create a surface
    context->eglSurface = eglCreateWindowSurface ( context->eglDisplay, config,
                                                    context->eglNativeWindow, NULL );
    
    if ( context->eglSurface == EGL_NO_SURFACE ){
        return GL_FALSE;
    }
    
    // Create a GL context
    context->eglContext = eglCreateContext ( context->eglDisplay, config,
                                              EGL_NO_CONTEXT, contextAttribs );
    
    if ( context->eglContext == EGL_NO_CONTEXT ){
        return GL_FALSE;
    }
    
    // Make the context current
    if ( !eglMakeCurrent ( context->eglDisplay, context->eglSurface,
                          context->eglSurface, context->eglContext ) ){
        return GL_FALSE;
    }
    
#endif // #ifndef __APPLE__
    
    return GL_TRUE;
}

void SRUTIL_API SRRegisterDrawFunc ( SRContext *context, void ( SRCALLBACK *drawFunc ) ( SRContext * ) ){
    context->drawFunc = drawFunc;
}

void SRUTIL_API SRRegisterShutdownFunc ( SRContext *context, void ( SRCALLBACK *shutdownFunc ) ( SRContext * ) ){
    context->shutdownFunc = shutdownFunc;
}

void SRUTIL_API SRRegisterUpdateFunc ( SRContext *context, void ( SRCALLBACK *updateFunc ) ( SRContext *, float ) ){
    context->updateFunc = updateFunc;
}

void SRUTIL_API SRRegisterTouchFunc ( SRContext *context, void ( SRCALLBACK *touchFunc ) ( SRContext *, int , int, int ) ){
    context->touchFunc = touchFunc;
}


#pragma mark LOGGING
void logMessage(const char *formatString, ...){
    va_list params;
    char buf[1024];
    va_start(params, formatString);
    vsprintf(buf, formatString, params);
#ifdef ANDROID
    __android_log_print( ANDROID_LOG_INFO, "SRUtils", "%s", buf);
#else
    printf("%s", buf);
#endif
    va_end(params);
}

#pragma mark FILE RELATED
srFile  *fileOpen       ( void *ioContext, const char *fileName ){
    srFile *pFile = NULL;
//    logMessage("File open start");
    
#ifdef ANDROID
    if ( ioContext != NULL ){
        AAssetManager *assetManager = ( AAssetManager * ) ((SRContext*)ioContext)->platformData;
        pFile = AAssetManager_open ( assetManager, fileName, AASSET_MODE_BUFFER );
    }
#else
#ifdef __APPLE__
    // iOS: Remap the filename to a path that can be opened from the bundle.
       fileName = GetBundleFileName ( fileName );
//    fileName = "";
#endif
    pFile = fopen ( fileName, "rb" );
#endif
//    logMessage("FILE OPEN");
    return pFile;
}

long getFileSize(srFile *pFile){
#ifdef ANDROID
    long fSize = AAsset_getLength(pFile);
#else
    fseek(pFile, 0, SEEK_END);
    long fSize = ftell(pFile);
    rewind(pFile);
#endif
    return fSize;
}

void    fileClose       ( srFile *pFile ){
    if ( pFile != NULL )
    {
#ifdef ANDROID
        AAsset_close ( pFile );
#else
        fclose ( pFile );
        pFile = NULL;
#endif
    }
}
int     fileRead        ( srFile *pFile, long bytesToRead, void *buffer ){
    int bytesRead = 0;
    
    if ( pFile == NULL ) return bytesRead;
    
#ifdef ANDROID
    bytesRead = AAsset_read ( pFile, buffer, bytesToRead );
#else
    bytesRead = fread ( buffer, bytesToRead, 1, pFile );
#endif
    
    return bytesRead;
}

#pragma mark LOAD PNG
std::vector<unsigned char> loadPNG ( void *ioContext, const char *fileName, unsigned &width, unsigned &height )
{
    
#ifdef __APPLE__
    // iOS: Remap the filename to a path that can be opened from the bundle.
    fileName = GetBundleFileName ( fileName );
#endif
    std::vector<unsigned char> image;
    unsigned error = lodepng::decode(image, width, height, fileName);
    if(error != 0){
        logMessage("Error: %s \n", lodepng_error_text(error) );
    }
    // Texture size must be power of two for the primitive OpenGL version this is written for. Find next power of two.
    size_t u2 = 1; while(u2 < width) u2 *= 2;
    size_t v2 = 1; while(v2 < height) v2 *= 2;
    // Ratio for power of two version compared to actual version, to render the non power of two image with proper size.
    double u3 = (double)width / u2;
    double v3 = (double)height / v2;
    // Make power of two version of the image.
    std::vector<unsigned char> image2(u2 * v2 * 4);
    for(size_t y = 0; y < height; y++)
        for(size_t x = 0; x < width; x++)
            for(size_t c = 0; c < 4; c++)
            {
                image2[4 * u2 * y + 4 * x + c] = image[4 * width * y + 4 * x + c];
            }
    return image2;
}

#pragma mark READ SHADER
std::unique_ptr<FileContent> readTextFile( void *ioContext, const char *fileName){
    srFile      *fp;
    // Open the file for reading
    fp = fileOpen ( ioContext, fileName );
    
    if ( fp == NULL ){
        // Log error as 'error in opening the input file from apk'
        logMessage ( "read file has FAILED to load : { %s }\n", fileName );
        return std::unique_ptr<FileContent>();
    }
    long fSize = getFileSize(fp);
    
    char* tempBuffer = new char[fSize + 1];
    int redBytes = fileRead ( fp, fSize, tempBuffer );
    
    fileClose(fp);
    tempBuffer[fSize] = 0;

    std::unique_ptr<FileContent> rValue = std::unique_ptr<FileContent>(new FileContent(tempBuffer, fSize + 1));
    return rValue;
}

#pragma mark READ OBJ FILE
std::unique_ptr<FileContent> readOBJFromFile(void *ioContext, const char *fileName){
    srFile *fp;
    fp = fileOpen(ioContext, fileName);
    if( fp == NULL){
        logMessage("readOBJFromFile FAILED to load : { %s }\n", fileName);
        return std::unique_ptr<FileContent>();
    }

    long fSize = getFileSize(fp);
    
    char* tempBuffer = new char[fSize];
    
    int redBytes = fileRead ( fp, fSize, tempBuffer );
    fileClose(fp);
    std::unique_ptr<FileContent> rValue = std::unique_ptr<FileContent>(new FileContent(tempBuffer, fSize));
    return rValue;
}

std::vector<unsigned char> loadRawPNGData(void *ioContext, const char *filename, unsigned int &width, unsigned int &height){

    srFile *fp;
    fp = fileOpen(ioContext, filename);
    if( fp == NULL){
        logMessage("loadRawPNGData FAILED to load file: { %s }\n", filename);
        return std::vector<unsigned char>();
    }
    std::vector<unsigned char> in;
    unsigned int fSize = getFileSize(fp);
    unsigned char* tempBuffer = new unsigned char[fSize];
    int redBytes = fileRead ( fp, fSize, tempBuffer );
    for(unsigned int i = 0; i < fSize; ++i){
        in.push_back(tempBuffer[i]);
    }
    fileClose(fp);
    if(tempBuffer) delete[] tempBuffer;
    
    std::vector<unsigned char> out;
    unsigned error = lodepng::decode(out, width, height, in);
//    if(error != 0){ logMessage("Error: %s \n", lodepng_error_text(error)); }
//    
//    size_t u2 = 1; while(u2 < width) u2 *= 2;
//    size_t v2 = 1; while(v2 < height) v2 *= 2;
//    unsigned char* rawData = new unsigned char[u2 * v2 * 4];
//    for(size_t y = 0; y < height; ++y){
//        for(size_t x = 0; x < width; ++x){
//            for(size_t c = 0; c < 4; ++c){
//                rawData[4 * u2 * y + 4 * x + c] = out[4 * width * y + 4 * x + c];
//            }
//        }
//    }
//
//    return rawData;
    return out;
}
