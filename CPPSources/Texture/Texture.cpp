#include "Texture.h"
#include "lodepng.h"


Texture::Texture(const char* filename,unsigned int target, unsigned int texelType, unsigned char byte, unsigned int format, unsigned int ID) : texelType(texelType), byte(4), format(format), target(target), ID(ID){
    logGLError();
    this->filename = filename;
    string file = filename;
#ifdef ANDROID
    file = "textures/" + file;
#endif
    texelArray = loadRawPNGData(file.c_str(), width, height);
    logGLError();
}

Texture::~Texture(){
}

std::shared_ptr<Texture> Texture::load(const char *filename, unsigned int target){
    logGLError();
    return std::shared_ptr<Texture>(std::shared_ptr<Texture>(new Texture(filename, target)));
}


void Texture::generateID(unsigned int flags, unsigned char filter){
    logGLError();
    if(ID) return;
 
    glGenTextures(1, &ID);
    glBindTexture(target, ID);
    
    if(flags & TEXTURE_CLAMP){
        glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }else{
        glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    
    if( flags & TEXTURE_MIPMAP ){
        switch( filter ){
            case TEXTURE_FILTER_1X:
                glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR );
                glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
                break;
            case TEXTURE_FILTER_2X:
                glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
                glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
                break;
            case TEXTURE_FILTER_3X:
                glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
                glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
                break;
            case TEXTURE_FILTER_0X:
            default:
                glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
                glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
                break;
        }
    }else{
        switch( filter ){
            case TEXTURE_FILTER_0X:
                glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
                glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
                break;
            default:
                glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
                glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
                break;
        }
    }
    
    glTexImage2D(target, 0, format, width, height, 0, format, texelType, &texelArray[0]);
    logGLError();
    
    // generateMipMap if needed...
}