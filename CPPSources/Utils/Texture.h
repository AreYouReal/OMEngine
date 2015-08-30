#pragma once

#include "SRUtils.h"

enum{ TEXTURE_CLAMP        = ( 1 << 0 ),
      TEXTURE_MIPMAP       = ( 1 << 1 ),
      TEXTURE_16_BITS      = ( 1 << 2 ),
      TEXTURE_16_BITS_5551 = ( 1 << 3 )};

enum{ TEXTURE_FILTER_0X = 0,
      TEXTURE_FILTER_1X = 1,
      TEXTURE_FILTER_2X = 2,
      TEXTURE_FILTER_3X = 3 };

struct Texture{
    Texture(void* context, const char* filename);
    ~Texture();
    
    void generateID(unsigned int flags, unsigned char filter);
    
    std::string    filename;
    unsigned int ID;           // The open GL texture id
    unsigned int width;
    unsigned int height;
    unsigned char byte;                 // The texture bits???
    
    unsigned int size;                  // The total size of the texture
    unsigned int target;                // Default GL_TEXTURE_2D
    unsigned int format;                // The format of the texture
    
    unsigned int texelType;            // The type of the texel to use for this texture( Default - GL_UNSIGNED_BYTE)
    
    unsigned char *texelArray;          // The raw texel array
//    unsigned int compression;           // The comperssion type
};
