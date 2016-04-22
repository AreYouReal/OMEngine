#include "WireSP.hpp"

void WireSP::initUniformLocations(){
    for(const auto &uniform : uniformArray){
        if(!uniform.name.compare(ShaderProgram::uniModelViewMatName)){
            modelViewMatUniLoc = uniform.location;
        }else if(!uniform.name.compare(ShaderProgram::uniProjectionMatName)){
            projectionMatUniLoc = uniform.location;
        }else if(!uniform.name.compare("uMaterial.ambient")){
            materialAmbient = uniform.location;
        }
    }
}

void WireSP::setUniforms(ObjMaterial *mat){
    m4d matrix = Camera::instance()->modelViewMatrix();
    glUniformMatrix4fv(modelViewMatUniLoc, 1, GL_TRUE, matrix.pointer());
    matrix = Camera::instance()->projectionMatrix();
    glUniformMatrix4fv(projectionMatUniLoc, 1, GL_TRUE, matrix.pointer());
    glUniform4fv(materialAmbient, 1, &mat->ambient.x);
}