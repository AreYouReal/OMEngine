#include "NormalSP.hpp"

void NormalSP::initUniformLocations(){
    for(const auto &uniform : uniformArray){
        if(!uniform.name.compare(ShaderProgram::uniModelViewMatName)){
            modelViewMatrixUniLoc = uniform.location;
        }else if(!uniform.name.compare(ShaderProgram::uniProjectionMatName)){
            projectionMatrixUniLoc = uniform.location;
        }
    }
}

void NormalSP::setUniforms(ObjMaterial *mat){
    m4d matrix = Camera::instance()->modelViewMatrix();
    glUniformMatrix4fv(modelViewMatrixUniLoc, 1, GL_TRUE, matrix.pointer());
    matrix = Camera::instance()->projectionMatrix();
    glUniformMatrix4fv(projectionMatrixUniLoc, 1, GL_TRUE, matrix.pointer());
}