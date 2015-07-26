#include "SRUtils.h"

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
    logMessage("File open start");
    
#ifdef ANDROID
    if ( ioContext != NULL ){
        AAssetManager *assetManager = ( AAssetManager * ) ((ESContext*)ioContext)->platformData;
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
    logMessage("FILE OPEN");
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
std::shared_ptr<ShaderSource> readShaderFromFile( void *ioContext, const char *fileName){
    srFile      *fp;
    // Open the file for reading
    fp = fileOpen ( ioContext, fileName );
    
    if ( fp == NULL ){
        // Log error as 'error in opening the input file from apk'
        logMessage ( "esReadShaderFromFile FAILED to load : { %s }\n", fileName );
        return nullptr;
    }
    long fSize = getFileSize(fp);
    char tempBuffer[fSize];
    int redBytes = fileRead ( fp, fSize, tempBuffer );
    logMessage("RED BYTES: { %d / %d }", redBytes, fSize);
    
    fileClose(fp);
    
    std::shared_ptr<ShaderSource> shaderSource = std::shared_ptr<ShaderSource>(new ShaderSource());
    
    shaderSource->source = new char[fSize + 1];
    memcpy(shaderSource->source, tempBuffer, fSize);
    shaderSource->size = fSize;
    shaderSource->source[fSize] = 0;
    for(int i = 0; i < fSize + 1; i++){
        logMessage("%c", shaderSource->source[i]);
    }
    return shaderSource;
}

#pragma mark READ OBJ FILE
void readOBJFromFile(void *ioContext, const char *fileName){
    srFile *fp;
    fp = fileOpen(ioContext, fileName);
    if( fp == NULL){
        logMessage("readOBJFromFile FAILED to load : { %s }\n", fileName);
        return;
    }
    
    long fSize = getFileSize(fp);
    
    char tempBuffer[fSize];
    
    int redBytes = fileRead ( fp, fSize, tempBuffer );
    fileClose(fp);
    for(long i = 0; i < fSize; i++){
        logMessage("%c", tempBuffer[i]);
    }
}