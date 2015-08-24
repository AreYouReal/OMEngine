#include "Texture.h"
#include "main.h"


Texture::Texture(void* context, const char* filename){
    texelArray = loadRawPNGData(context, filename, width, height);

    logMessage("%d, %d\n", width, height);
    
    strcpy(this->filename, filename);
    
    
    
}

Texture::~Texture(){
    if(texelArray) delete[] texelArray;
    
    logMessage("Texture destructor");
}

Texture* Texture::create(void* context, char* filename){

    Texture txt(context, filename);
    
    return nullptr;
}