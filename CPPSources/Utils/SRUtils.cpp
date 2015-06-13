#include "SRUtils.h"


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


static srFile *esFileOpen ( void *ioContext, const char *fileName ){
    srFile *pFile = NULL;
    
#ifdef ANDROID
    if ( ioContext != NULL ){
        AAssetManager *assetManager = ( AAssetManager * ) ioContext;
        pFile = AAssetManager_open ( assetManager, fileName, AASSET_MODE_BUFFER );
    }
#else
#ifdef __APPLE__
    // iOS: Remap the filename to a path that can be opened from the bundle.
    //   fileName = GetBundleFileName ( fileName );
    fileName = "";
#endif
    pFile = fopen ( fileName, "rb" );
#endif
    
    return pFile;
}


static void esFileClose ( srFile *pFile ){
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

static int esFileRead ( srFile *pFile, int bytesToRead, void *buffer ){
    int bytesRead = 0;
    
    if ( pFile == NULL ) return bytesRead;
    
#ifdef ANDROID
    bytesRead = AAsset_read ( pFile, buffer, bytesToRead );
#else
    bytesRead = fread ( buffer, bytesToRead, 1, pFile );
#endif
    
    return bytesRead;
}