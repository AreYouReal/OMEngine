#pragma once

#include "SRUtils.h"


struct Texture{
    static Texture* create(char *filename);
    
    char    filename[MAX_CHAR];
    unsigned int tID;           // The open GL texture id
    unsigned short width;
    unsigned short height;
    
    unsigned char byte; // The texture bits???
    
    unsigned int size;          // The total size of the texture
    unsigned int target;        // Default GL_TEXTURE_2D
    
    unsigned int internalFormat;        // The internal format of the texture
    unsigned int format;                // The format of the texture
    
    unsigned int textelType;            // The type of the texel to use for this texture( Default - GL_UNSIGNED_BYTE)
    
    unsigned char *texelArray;          // The raw texel array
    unsigned int compression;           // The comperssion type
};
