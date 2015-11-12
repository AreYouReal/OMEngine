
#include "Font.hpp"

#include "Game.h"


Font::Font(string name, float fSize, unsigned int tWidth, unsigned int tHeight, int firstChar, int charCount){
    logMessage("Font constructor! %s\n", name.c_str());
    up<FileContent> content = readBytesFromFile(Game::getAppContext(), name.c_str());
    if(!content){
        logMessage("Unable to load font file: %s\n", name.c_str());
    }else{
        unsigned char *texelArray = new unsigned char[tWidth * tHeight];
        charData = new stbtt_bakedchar[charCount];
        fontSize = fSize;
        textureWidth = tWidth;
        textureHeight = tHeight;
        firstCharacter = firstChar;
        characterCount = charCount;
        
        stbtt_BakeFontBitmap((unsigned char*)content->content, 0, fontSize, texelArray, textureWidth, textureHeight, firstCharacter, characterCount, charData);
        
        glGenTextures(1, &tID);
        glBindTexture(GL_TEXTURE_2D, tID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, textureWidth, textureHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, texelArray);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        delete [] texelArray;
    }
}


Font::~Font(){
    logMessage("Font destructor!\n");
}

float Font::length(const string& text){
    float l = 0;
    for(ushort i = 0; i < text.length(); ++i){
        if(text[i] >= firstCharacter && text[i] <= firstCharacter + characterCount){
            stbtt_bakedchar *backedChar = charData + text[i] - firstCharacter;
            l += backedChar->xadvance;
        }
    }
    return l;
}