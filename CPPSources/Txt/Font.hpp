#pragma once

#include "stb_truetype.h"
#include "ShaderProgram.h"
#include "Shortcuts.h"

class Font{
public:
    Font(string name, float fSize, unsigned int tWidth, unsigned int tHeight, int firstChar, int charCount);
    ~Font();
    
    
    float   size(){ return fontSize; };
    float   length(const string& text);
    void    print(float x, float y,const string &text, v4d *color = nullptr );
    
private:
    string              name;
    stbtt_bakedchar     *charData;
    float               fontSize;
    int                 textureWidth;
    int                 textureHeight;
    int                 firstCharacter;
    int                 characterCount;
    sp<ShaderProgram>   program;
    unsigned int        tID;
};