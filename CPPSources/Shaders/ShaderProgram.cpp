#include "ShaderProgram.h"
#include "Camera.h"
#include "Illuminator.hpp"

ShaderProgram::ShaderProgram(){
    logMessage("Shader program constructor!\n");
}

ShaderProgram::~ShaderProgram(){
    logMessage("Shader program destructor: %d\n", ID );
}


int ShaderProgram::getUniformLocation(const char *name){
    for(int i = 0; i < uniformArray.size(); i++) if(!strcmp(uniformArray[i].name.c_str(), name)) return uniformArray[i].location;
    
    return -1;
}

int ShaderProgram::getVertexAttribLocation(const char *name){
    for(int i = 0; i < attribArray.size(); i++) if(!strcmp(attribArray[i].name.c_str(), name)) return attribArray[i].location;
    
    return -1;
}

void ShaderProgram::use(){
    glUseProgram(ID);
    
    bindAttributes();
}

void ShaderProgram::bindAttributes(){
    glBindAttribLocation(ID, 0, "aPosition");
    glBindAttribLocation(ID, 1, "aNormal");
    glBindAttribLocation(ID, 2, "aTexCoord");
    glBindAttribLocation(ID, 3, "aTangent");
}

void ShaderProgram::setUniforms(ObjMaterial *mat){
    m4d matrix;
    
    for(unsigned short i = 0; i < uniformArray.size(); ++i){
        if(!strcmp(uniformArray[i].name.c_str(), "uSamplerDiffuse")){
            glUniform1i(uniformArray[i].location, 1);
        }else if(!strcmp(uniformArray[i].name.c_str(), "uModelViewM")){
            matrix = Camera::instance()->modelViewMatrix();
            glUniformMatrix4fv(uniformArray[i].location, 1, GL_TRUE, matrix.pointer());
        }else if(!strcmp(uniformArray[i].name.c_str(), "uProjectionM")){
            matrix = Camera::instance()->projectionMatrix();
            glUniformMatrix4fv(uniformArray[i].location, 1, GL_TRUE, matrix.pointer());
        }else if(!strcmp(uniformArray[i].name.c_str(), "uNormalM")){
            matrix = Camera::instance()->normalMatrix();
            glUniformMatrix4fv(uniformArray[i].location, 1, GL_TRUE, matrix.pointer());
        }else if(!strcmp(uniformArray[i].name.c_str(), "uDissolve")){
            glUniform1f(uniformArray[i].location, mat->dissolve);
        }else if(!strcmp(uniformArray[i].name.c_str(), "uMaterial.ambient")){
            glUniform4fv(uniformArray[i].location, 1, &mat->ambient.x);
        }else if(!strcmp(uniformArray[i].name.c_str(), "uMaterial.diffuse")){
            glUniform4fv(uniformArray[i].location, 1, &mat->diffuse.x);
        }else if(!strcmp(uniformArray[i].name.c_str(), "uMaterial.specular")){
            glUniform4fv(uniformArray[i].location, 1, &mat->specular.x);
        }else if(!strcmp(uniformArray[i].name.c_str(), "uMaterial.shininess")){
            glUniform1f(uniformArray[i].location, mat->specularExponent );
        }else if(!strcmp(uniformArray[i].name.c_str(), "uLight.position")){
            // LIGHT
        }else if(!strcmp(uniformArray[i].name.c_str(), "uSamplerBump")){
            glUniform1i(uniformArray[i].location, 4);
        }
    }
    
    char tmp[128] = {""};
    for(int i = 0; i < 2; ++i){
        sp<LightSource> light =Illuminator::instance()->getLightSource(i);
        v4d lightInEyeSpace = light->getPositionInEyeSpace();
        v4d color = light->getColor();
        v4d directionInEyeSpace = light->getDirectionInEyeSpace();
        
        sprintf(tmp, "uLightFS[%d].color", i);
        glUniform4fv(getUniformLocation(tmp), 1, &color.x);
        
        sprintf(tmp, "uLight[%d].type", i);
        glUniform1i(getUniformLocation(tmp), light->type());
        
        sprintf(tmp, "uLightFS[%d].type", i);
        glUniform1i(getUniformLocation(tmp), light->type());
        
        sprintf(tmp, "uLightFS[%d].spotCosCutoff", i);
        glUniform1f(getUniformLocation(tmp),  light->spotCosCutoff);
        
        sprintf(tmp, "uLightFS[%d].spotBlend", i);
        glUniform1f(getUniformLocation("uLightFS.spotBlend"),      light->spotBlend);
        
        
        sprintf(tmp, "uLightFS[%d].dst", i);
        glUniform1f(getUniformLocation(tmp),         light->distance());
        
        sprintf(tmp, "uLightFS[%d].linAtten", i);
        glUniform1f(getUniformLocation(tmp),    light->linearAtten());
        
        
        sprintf(tmp, "uLightFS[%d].quadAten", i);
        glUniform1f(getUniformLocation("uLightFS.quadAtent"),   light->quadAtten());
        
        sprintf(tmp, "uLight[%d].direction", i);
        glUniform3fv(getUniformLocation(tmp), 1, &directionInEyeSpace.x);
        
        sprintf(tmp, "uLight[%d].position", i);
        glUniform3fv(getUniformLocation(tmp), 1, &lightInEyeSpace.x);
    }
}