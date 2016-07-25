#include "ObjMaterial.h"
#include "ShaderProgram.h"

ObjMaterial::ObjMaterial(std::string name){
    logGLError();
    this->name = name;
}

ObjMaterial::~ObjMaterial(){
}

void ObjMaterial::use(){
    if(program){
        program->use();
        program->setUniforms(this);
    }
    
    if(tAmbient){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(tAmbient->target, tAmbient->ID);
    }
    if(tDiffuse){
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(tDiffuse->target, tDiffuse->ID);
    }
    if(tSpecular){
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(tSpecular->target, tSpecular->ID);
    }
    if(tDisp){
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(tDisp->target, tDisp->ID);
    }
    if(tBump){
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(tBump->target, tBump->ID);
    }
    if(tTranslucency){
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(tTranslucency->target, tTranslucency->ID);
    }

}

void ObjMaterial::loadTextures(){
    logGLError();
    if(mapAmbient[0]){
        sp<Texture> txt(Materials::instance()->getTexture(mapAmbient));
        if(txt != nullptr) tAmbient = txt;
    }
    if(mapDiffuse[0]){
        sp<Texture> txt(Materials::instance()->getTexture(mapDiffuse));
        if(txt != nullptr) tDiffuse = txt;
    }
    
    if(mapSpecular[0]){
        sp<Texture> txt(Materials::instance()->getTexture(mapSpecular));
        if(txt != nullptr) tSpecular = txt;
    }
    
    if(mapTranslucency[0]){
        sp<Texture> txt(Materials::instance()->getTexture(mapTranslucency));
        if(txt != nullptr) tTranslucency = txt;
    }
    
    if(mapDisp[0]){
        sp<Texture> txt(Materials::instance()->getTexture(mapDisp));
        if(txt != nullptr) tDisp = txt;
    }

    if(mapBump[0]){
        sp<Texture> txt(Materials::instance()->getTexture(mapBump));
        if(txt != nullptr) tBump = txt;
    }
}