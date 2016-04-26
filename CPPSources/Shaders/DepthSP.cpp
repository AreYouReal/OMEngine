#include "DepthSP.hpp"

void DepthSP::initUniformLocations(){
    modelViewM = getUniformLocation(ShaderProgram::uniModelViewMatName);
    projectionM = getUniformLocation(ShaderProgram::uniProjectionMatName);
}

void DepthSP::setUniforms(const ObjMaterial *mat){
    glUniformMatrix4fv(modelViewM, 1, GL_TRUE, Camera::instance()->modelViewMatrix().pointer());
    glUniformMatrix4fv(projectionM, 1, GL_TRUE, Camera::instance()->projectorMatrix().pointer());
}