#pragma once

#include "stb_truetype.h"
#include "SRUtils.h"
#include "ShaderProgram.h"
#include "Shortcuts.h"


class Font{
public:
    Font(string name, float fSize, unsigned int tWidth, unsigned int tHeight, int firstChar, int charCount);
    ~Font();
    
    float length(const string& text);
    
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