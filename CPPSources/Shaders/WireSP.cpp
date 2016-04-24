#include "WireSP.hpp"

void WireSP::initUniformLocations(){
    for(const auto &uniform : uniforms){
        if(!uniform.second.name.compare(ShaderProgram::uniModelViewMatName)){
            modelViewMatUniLoc = uniform.second.location;
        }else if(!uniform.second.name.compare(ShaderProgram::uniProjectionMatName)){
            projectionMatUniLoc = uniform.second.location;
        }else if(!uniform.second.name.compare("uMaterial.ambient")){
            materialAmbient = uniform.second.location;
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