#include "SkyboxSP.hpp"



void SkyboxSP::initUniformLocations(){
    for(const auto &uniform : uniforms){
        if(!uniform.second.name.compare(ShaderProgram::uniModelViewMatName)){
            modelViewMatrixUniLoc = uniform.second.location;
        }else if(!uniform.second.name.compare(ShaderProgram::uniProjectionMatName)){
            projectionMatrixUniLoc = uniform.second.location;
        }else if(!uniform.second.name.compare("uniformCubeSampler")){
            cubemapSamplerLocation = uniform.second.location;
        }
    }
}

void SkyboxSP::setUniforms(const ObjMaterial *mat){
    m4d matrix = Camera::instance()->modelViewMatrix();
    glUniformMatrix4fv(modelViewMatrixUniLoc, 1, GL_TRUE, matrix.pointer());
    matrix = Camera::instance()->projectionMatrix();
    glUniformMatrix4fv(projectionMatrixUniLoc, 1, GL_TRUE, matrix.pointer());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mat->tAmbient->ID);
}