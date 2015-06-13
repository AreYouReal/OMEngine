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
       fileName = GetBundleFileName ( fileName );
//    fileName = "";
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


std::vector<unsigned char> esLoadPNG ( void *ioContext, const char *fileName, unsigned &width, unsigned &height )
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

char* readShaderFromFile( void *ioContext, const char *fileName){
    srFile      *fp;
    char tempBuffer[4096];
    // Open the file for reading
    fp = esFileOpen ( ioContext, fileName );
    
    if ( fp == NULL ){
        // Log error as 'error in opening the input file from apk'
        logMessage ( "esReadShaderFromFile FAILED to load : { %s }\n", fileName );
        return nullptr;
    }
    int redBytes = esFileRead ( fp, 1, tempBuffer );
    int fileSize = redBytes;
    while(redBytes){
        redBytes = esFileRead ( fp, 1, tempBuffer + fileSize );
        fileSize += redBytes;
    }
    esFileClose(fp);
    logMessage("FIlesize: %d", fileSize);
    return nullptr;
}