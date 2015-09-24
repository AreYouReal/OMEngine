#include "ObjMaterial.h"
#include "Camera.h"
#include "Illuminator.hpp"

ObjMaterial::ObjMaterial(std::string name){
    this->name = name;
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

void ObjMaterial::setUniforms(){
    m4d matrix;
    for(unsigned short i = 0; i < program->uniformArray.size(); ++i){
        if(!strcmp(program->uniformArray[i].name.c_str(), "uSamplerDiffuse")){
            glUniform1i(program->uniformArray[i].location, 1);
        }else if(!strcmp(program->uniformArray[i].name.c_str(), "uModelViewM")){
            matrix = Camera::instance()->viewMatrix();
            glUniformMatrix4fv(program->uniformArray[i].location, 1, GL_TRUE, matrix.pointer());
        }else if(!strcmp(program->uniformArray[i].name.c_str(), "uProjectionM")){
            matrix = Camera::instance()->projectionMatrix();
            glUniformMatrix4fv(program->uniformArray[i].location, 1, GL_TRUE, matrix.pointer());
        }else if(!strcmp(program->uniformArray[i].name.c_str(), "uNormalM")){
            matrix = Camera::instance()->normalMatrix();
            glUniformMatrix4fv(program->uniformArray[i].location, 1, GL_TRUE, matrix.pointer());
        }else if(!strcmp(program->uniformArray[i].name.c_str(), "uDissolve")){
            glUniform1f(program->uniformArray[i].location, dissolve);
        }else if(!strcmp(program->uniformArray[i].name.c_str(), "uAmbient")){
            glUniform3fv(program->uniformArray[i].location, 1, &ambient.x);
        }else if(!strcmp(program->uniformArray[i].name.c_str(), "uDiffuse")){
            glUniform3fv(program->uniformArray[i].location, 1, &diffuse.x);
        }else if(!strcmp(program->uniformArray[i].name.c_str(), "uSpecular")){
            glUniform3fv(program->uniformArray[i].location, 1, &specular.x);
        }else if(!strcmp(program->uniformArray[i].name.c_str(), "uShininess")){
            glUniform1f(program->uniformArray[i].location, specularExponent);
        }else if(!strcmp(program->uniformArray[i].name.c_str(), "uLightPos")){
            v4d lightInEyeSpace = Illuminator::instance()->getLightSource()->getPosition();
            glUniform3fv(program->uniformArray[i].location, 1, &lightInEyeSpace.x);
        }else if(!strcmp(program->uniformArray[i].name.c_str(), "uSamplerBump")){
            glUniform1i(program->uniformArray[i].location, 4);
        }
    }
}