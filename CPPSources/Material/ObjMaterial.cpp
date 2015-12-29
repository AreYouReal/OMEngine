#include "ObjMaterial.h"
#include "Camera.h"
#include "Illuminator.hpp"

ObjMaterial::ObjMaterial(std::string name){
    this->name = name;
    logMessage("ObjMaterial constructor!%s\n", name.c_str());
}

ObjMaterial::~ObjMaterial(){
    logMessage("ObjMaterial destructor!%s\n", name.c_str());
}

void ObjMaterial::use(){
    if(program) program->use();
    
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
    
    setUniforms();
}

void ObjMaterial::setProgram(sp<ShaderProgram> prog){
    if(program == nullptr) program = prog;
}

void ObjMaterial::loadTextures(){
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

v4d lightColor(1.0, 1.0, 1.0, 1.0);

void ObjMaterial::setUniforms(){
    m4d matrix;
    for(unsigned short i = 0; i < program->uniformArray.size(); ++i){
        if(!strcmp(program->uniformArray[i].name.c_str(), "uSamplerDiffuse")){
            glUniform1i(program->uniformArray[i].location, 1);
        }else if(!strcmp(program->uniformArray[i].name.c_str(), "uModelViewM")){
            matrix = Camera::instance()->modelViewMatrix();
            glUniformMatrix4fv(program->uniformArray[i].location, 1, GL_TRUE, matrix.pointer());
        }else if(!strcmp(program->uniformArray[i].name.c_str(), "uProjectionM")){
            matrix = Camera::instance()->projectionMatrix();
            glUniformMatrix4fv(program->uniformArray[i].location, 1, GL_TRUE, matrix.pointer());
        }else if(!strcmp(program->uniformArray[i].name.c_str(), "uNormalM")){
            matrix = Camera::instance()->normalMatrix();
            glUniformMatrix3fv(program->uniformArray[i].location, 1, GL_TRUE, matrix.pointer());
        }else if(!strcmp(program->uniformArray[i].name.c_str(), "uDissolve")){
            glUniform1f(program->uniformArray[i].location, dissolve);
        }else if(!strcmp(program->uniformArray[i].name.c_str(), "uMaterial.ambient")){
            glUniform4fv(program->uniformArray[i].location, 1, &ambient.x);
        }else if(!strcmp(program->uniformArray[i].name.c_str(), "uMaterial.diffuse")){
            glUniform4fv(program->uniformArray[i].location, 1, &diffuse.x);
        }else if(!strcmp(program->uniformArray[i].name.c_str(), "uMaterial.specular")){
            glUniform4fv(program->uniformArray[i].location, 1, &specular.x);
        }else if(!strcmp(program->uniformArray[i].name.c_str(), "uMateril.shininess")){
            glUniform1f(program->uniformArray[i].location, specularExponent * 0.128f);
        }else if(!strcmp(program->uniformArray[i].name.c_str(), "uLight.position")){
            v4d lightInEyeSpace = Illuminator::instance()->getLightSource()->getPositionInEyeSpace();
            glUniform3fv(program->uniformArray[i].location, 1, &lightInEyeSpace.x);
            glUniform4fv(program->getUniformLocation("uLight.color"), 1, &lightColor.x);
        }else if(!strcmp(program->uniformArray[i].name.c_str(), "uSamplerBump")){
            glUniform1i(program->uniformArray[i].location, 4);
        }else if(!strcmp(program->uniformArray[i].name.c_str(), "uColor")){
            v3d color(1, 0, 0);
            glUniform3fv(program->uniformArray[i].location, 1, &color.x);
        }
    }
}