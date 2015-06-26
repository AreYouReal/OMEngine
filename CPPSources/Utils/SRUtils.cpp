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
    logMessage("FILE OPEN");
    return pFile;
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
int     fileRead        ( srFile *pFile, int bytesToRead, void *buffer ){
    int bytesRead = 0;
    
    if ( pFile == NULL ) return bytesRead;
    
#ifdef ANDROID
    bytesRead = AAsset_read ( pFile, buffer, bytesToRead );
#else
    bytesRead = fread ( buffer, bytesToRead, 1, pFile );
#endif
    
    return bytesRead;
}
void    adjustFilePath  (char *filepath){
    unsigned int i = 0, l = strlen(filepath);
    while( i != l){
        if(filepath[i] == '\\') filepath[ i ] = '/';
        ++i;
    }
}
void    getFilePath     (char *filepath, char *path){
    char *t = NULL;
    unsigned int p;
    adjustFilePath( filepath );
    t = strrchr(filepath, '/');
    if( t ){
        p = ( t - filepath ) + 1;
        strncpy( path, filepath, p);
        path[ p ] = 0;
    }
}
void    getFileName     (char *filepath, char *filename){
    char *t = NULL;
    adjustFilePath(filepath);
    t = strrchr(filepath, '/');
    if( t ) strcpy(filename, t + 1);
    else strcpy(filename, filepath);
}
void    getFileExtension(char *filepath, char *ext, bool uppercase){
    char *t = NULL;
    adjustFilePath(filepath);
    t = strrchr(filepath, '.');
    if( t ) strcpy(ext, t + 1);
    if(uppercase){
        unsigned int i = 0, l = strlen( ext );
        while(i != l){
            ext[ i ] = toupper(ext[i]);
            ++i;
        }
    }
}

Memory  *mopen          (char *filename, bool relative_path){
#ifdef __IPHONE_4_0
    FILE *f;
    char fname[MAX_PATH] = {""};
    if(relative_path){
        getFilePath(getenv( "FILESYSTEM" ), fname);
        strcat(fname, filename);
    }else{
        strcpy(fname, filename);
    }
    
    f = fopen(fname, "rb");
    if( !f ) return NULL;
    
    Memory *memory = (Memory *) calloc(1, sizeof(Memory));
    strcpy(memory->filename, fname);
    fseek(f, 0, SEEK_END);
    memory->size = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    memory->buffer = (unsigned char * ) calloc(1, memory->size + 1);
    fread(memory->buffer, memory->size, 1, f);
    memory->buffer[memory->size] = 0;
    fclose(f);
    return memory;
#else
    char    fpath[MAX_PATH] = {""},
            fname[MAX_PATH] = {""};
    
    unzFile		    uf;
    unz_file_info   fi;
    unz_file_pos    fp;

    strcpy( fpath, getenv( "FILESYSTEM" ) );
    
    uf = unzOpen( fpath );
    
    if( !uf ) return NULL;
    
    if( relative_path ) sprintf( fname, "assets/%s", filename );
    else strcpy( fname, filename );
    
    unzGoToFirstFile( uf );
    
    Memory *memory = ( Memory * ) calloc( 1, sizeof( Memory ) );
    
    unzGetFilePos( uf, &fp );
    
    if( unzLocateFile( uf, fname, 1 ) == UNZ_OK ){
        unzGetCurrentFileInfo(  uf,
                              &fi,
                              memory->filename,
                              MAX_PATH,
                              NULL, 0,
                              NULL, 0 );
        
        if( unzOpenCurrentFilePassword( uf, NULL ) == UNZ_OK ){
            memory->position = 0;
            memory->size	 = fi.uncompressed_size;
            memory->buffer   = ( unsigned char * ) realloc( memory->buffer, fi.uncompressed_size + 1 );
            memory->buffer[ fi.uncompressed_size ] = 0;
            
            while( unzReadCurrentFile( uf, memory->buffer, fi.uncompressed_size ) > 0 ){}
            
            unzCloseCurrentFile( uf );
            
            unzClose( uf );
            
            return memory;
        }
    }
    
    unzClose( uf );
    
    return NULL;
    
#endif
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
    char tempBuffer[4096];
    // Open the file for reading
    fp = fileOpen ( ioContext, fileName );
    
    if ( fp == NULL ){
        // Log error as 'error in opening the input file from apk'
        logMessage ( "esReadShaderFromFile FAILED to load : { %s }\n", fileName );
        return nullptr;
    }
    int redBytes = fileRead ( fp, 1, tempBuffer );
    int fileSize = redBytes;
    while(redBytes){
        redBytes = fileRead ( fp, 1, tempBuffer + fileSize );
        fileSize += redBytes;
    }
    fileClose(fp);
    std::shared_ptr<ShaderSource> shaderSource = std::shared_ptr<ShaderSource>(new ShaderSource());
    shaderSource->source = new char[fileSize + 1];
    memcpy(shaderSource->source, tempBuffer, fileSize);
    shaderSource->source[fileSize] = 0;
    for(int i = 0; i < fileSize + 1; i++){
        logMessage("%c", shaderSource->source[i]);
    }
    return shaderSource;
}