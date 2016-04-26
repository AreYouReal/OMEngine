#include "ShaderProgram.h"

// Attributes
const string ShaderProgram::posAttribName       {"aPosition"};
const string ShaderProgram::normAttribName      {"aNormal"  };
const string ShaderProgram::texCoordAttribName  {"aTexCoord"};
const string ShaderProgram::tanAttribName       {"aTangent" };

// Uniform
// Matrices
const string ShaderProgram::uniModelViewMatName {"uModelViewM"};
const string ShaderProgram::uniProjectionMatName{"uProjectionM"};
const string ShaderProgram::uniNormalMName      {"uNormalM"};
const string ShaderProgram::uniShadowMName      {"uShadowM"};


ShaderProgram::ShaderProgram(){
    logGLError();
}

ShaderProgram::~ShaderProgram(){
}

void ShaderProgram::initUniformLocations(){
    for(auto const &uniform : uniforms){
        logMessage("Uniform: %s\n",uniform.second.name.c_str());
    }
    // Shader specific
}

void ShaderProgram::use(){
    glUseProgram(ID);
    
//    bindAttributes();
}

// FOR REFERENCE -> No need to bind manually -> layout(location = 0) modifier in shader
//void ShaderProgram::bindAttributes(){
////    glBindAttribLocation(ID, attribLocations[Attributes::POSITION], "aPosition");
////    glBindAttribLocation(ID, attribLocations[Attributes::NORMAL],   "aNormal");
////    glBindAttribLocation(ID, attribLocations[Attributes::TEXTURE],  "aTexCoord");
////    glBindAttribLocation(ID, attribLocations[Attributes::TANGENT],  "aTangent");
//}

void ShaderProgram::setUniforms(const ObjMaterial *mat){
    m4d matrix;
    
    for(auto const &entry : uniforms){
        if(!strcmp(entry.second.name.c_str(), "uSamplerDiffuse")){
            glUniform1i(entry.second.location, 1);
        }else if(!strcmp(entry.second.name.c_str(), "uModelViewM")){
            matrix = Camera::instance()->modelViewMatrix();
            glUniformMatrix4fv(entry.second.location, 1, GL_TRUE, matrix.pointer());
        }else if(!strcmp(entry.second.name.c_str(), "uProjectionM")){
            matrix = Camera::instance()->projectionMatrix();
            glUniformMatrix4fv(entry.second.location, 1, GL_TRUE, matrix.pointer());
        }else if(!strcmp(entry.second.name.c_str(), "uNormalM")){
            matrix = Camera::instance()->normalMatrix();
            glUniformMatrix4fv(entry.second.location, 1, GL_TRUE, matrix.pointer());
        }else if(!strcmp(entry.second.name.c_str(), "uDissolve")){
            glUniform1f(entry.second.location, mat->dissolve);
        }else if(!strcmp(entry.second.name.c_str(), "uMaterial.ambient")){
            glUniform4fv(entry.second.location, 1, &mat->ambient.x);
        }else if(!strcmp(entry.second.name.c_str(), "uMaterial.diffuse")){
            glUniform4fv(entry.second.location, 1, &mat->diffuse.x);
        }else if(!strcmp(entry.second.name.c_str(), "uMaterial.specular")){
            glUniform4fv(entry.second.location, 1, &mat->specular.x);
        }else if(!strcmp(entry.second.name.c_str(), "uMaterial.shininess")){
            glUniform1f(entry.second.location, mat->specularExponent );
        }else if(!strcmp(entry.second.name.c_str(), "uLight.position")){
            // LIGHT
        }else if(!strcmp(entry.second.name.c_str(), "uSamplerBump")){
            glUniform1i(entry.second.location, 4);
        }else if(!strcmp(entry.second.name.c_str(), "uSamplerProjector")){
            glUniform1i(entry.second.location, 0);
        }else if(!strcmp(entry.second.name.c_str(), "uProjM")){
            matrix = Camera::instance()->projectorMatrix();
            glUniformMatrix4fv(entry.second.location, 1, GL_TRUE, matrix.pointer());
        }

    }
    
    char tmp[128] = {""};
    for(int i = 0; i < 1; ++i){
        sp<LightSource> light =Illuminator::instance()->getLightSource(i);
        v4d lightInEyeSpace = light->getPositionInEyeSpace();
        v4d color = light->getColor();
        v4d directionInEyeSpace = light->getDirectionInEyeSpace();

        sprintf(tmp, "uLightFS[%d].color", i);
        string qwe = tmp;
        
        glUniform4fv(getUniformLocation(qwe), 1, &color.x);
        
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


int ShaderProgram::getUniformLocation(const string &name){
    if(uniforms.find(name) != uniforms.end()){
        return uniforms[name].location;
    }else{
        logMessage("Can't find uniform %s!!!\n", name.c_str());
        return -1;
    }
}