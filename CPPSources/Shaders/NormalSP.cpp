#include "NormalSP.hpp"


void NormalSP::initAttribLocations(){
    for(const auto &attrib : attribArray){
        if(!attrib.name.compare(posAttribName)){
            attribLocations[Attributes::POSITION] = attrib.location;
        }else if(!attrib.name.compare(normAttribName)){
            attribLocations[Attributes::NORMAL] = attrib.location;
        }
    }
}

void NormalSP::initUniformLocations(){
    for(const auto &uniform : uniformArray){
        if(!uniform.name.compare(ShaderProgram::uniModelViewMatName)){
            modelViewMatrixUniLoc = uniform.location;
        }else if(!uniform.name.compare(ShaderProgram::uniProjectionMatName)){
            projectionMatrixUniLoc = uniform.location;
        }
    }
}


//void NormalSP::bindAttributes(){
////    glBindAttribLocation(ID, 0, "aPosition");
////    glBindAttribLocation(ID, 0,   "aNormal");
//}


void NormalSP::setUniforms(ObjMaterial *mat){
    m4d matrix = Camera::instance()->modelViewMatrix();
    glUniformMatrix4fv(modelViewMatrixUniLoc, 1, GL_TRUE, matrix.pointer());
    matrix = Camera::instance()->projectionMatrix();
    glUniformMatrix4fv(projectionMatrixUniLoc, 1, GL_TRUE, matrix.pointer());
}