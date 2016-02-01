
#include "Font.hpp"
#include "Materials.hpp"
#include "Camera.h"


Font::Font(string name, float fSize, unsigned int tWidth, unsigned int tHeight, int firstChar, int charCount){
#ifdef ANDROID
    name = "fonts/" + name;
#endif
    up<FileContent> content = readBytesFromFile( name.c_str());
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
        
        program = Materials::instance()->getProgram("font");
    }
}


Font::~Font(){
}

float Font::length(const string& text){
    float l = 0;
    for(unsigned short i = 0; i < text.length(); ++i){
        if(text[i] >= firstCharacter && text[i] <= firstCharacter + characterCount){
            stbtt_bakedchar *backedChar = charData + text[i] - firstCharacter;
            l += backedChar->xadvance;
        }
    }
    return l;
}

void Font::print(float x, float y, const string &text, v4d *color){
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    glDisable   (GL_CULL_FACE);
    glDisable   (GL_DEPTH_TEST);
    glDepthMask (GL_FALSE);
    glEnable    (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    program->use();
    m4d m = Camera::instance()->orthoMatrix();
    glUniformMatrix4fv(program->getUniformLocation("uProjectionM"), 1, GL_TRUE, m.pointer());
    if(color) glUniform4fv(program->getUniformLocation("uColor"), 1, (*color).pointer());
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tID);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(2);
    
    for(unsigned short i = 0; i < text.length(); ++i){
        if(text[i] >= firstCharacter && text[i] <= firstCharacter + characterCount){
            v2d v[4];
            v2d uv[4];
            
            stbtt_aligned_quad quad;
            stbtt_bakedchar *backedchar = charData + (text[i] - firstCharacter);
            int roundX = floorf(x + backedchar->xoff);
            int roundY = floorf(y - backedchar->yoff);
            
            quad.x0 = (float)roundX;
            quad.y0 = (float)roundY;
            quad.x1 = (float)roundX + backedchar->x1 - backedchar->x0;
            quad.y1 = (float)roundY - backedchar->y1 + backedchar->y0;
            
            quad.s0 = backedchar->x0 / (float)textureWidth;
            quad.t0 = backedchar->y0 / (float)textureHeight;
            quad.s1 = backedchar->x1 / (float)textureWidth;
            quad.t1 = backedchar->y1 / (float)textureHeight;
            
            x += backedchar->xadvance;
            
            v [0].x = quad.x1; v [0].y = quad.y0;
            uv[0].x = quad.s1; uv[0].y = quad.t1;

            v [1].x = quad.x0; v [1].y = quad.y0;
            uv[1].x = quad.s0; uv[1].y = quad.t1;
            
            v [2].x = quad.x1; v [2].y = quad.y1;
            uv[2].x = quad.s1; uv[2].y = quad.t0;
            
            v [3].x = quad.x0; v [3].y = quad.y1;
            uv[3].x = quad.s0; uv[3].y = quad.t0;
            
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (float *)&v[0]);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (float *)&uv[0]);
            
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
    }
    
    glEnable    (GL_CULL_FACE);
    glEnable    (GL_DEPTH_TEST);
    glDepthMask (GL_TRUE);
    glDisable   (GL_BLEND);
}