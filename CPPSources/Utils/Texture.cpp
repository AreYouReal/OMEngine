#include "Texture.h"
#include "main.h"


Texture* Texture::create(char* filename){
    unsigned int width, height;
    unsigned char* rawData = loadRawPNGData(SRGraphics::getAppContext(), filename, width, height);
    
    logMessage("%d x %d", width, height);
    
    if(rawData) delete [] rawData;
    
    return nullptr;
}
