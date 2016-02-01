#include "GreySP.hpp"


void GreySP::initAttribLocations(){
    NormalSP::initAttribLocations();
}

void GreySP::initUniformLocations(){
    for(const auto &uniform : uniformArray){
        if(!uniform.name.compare(ShaderProgram::uniModelViewMatName)){
            modelViewMatrixUniLoc = uniform.location;
        }else if(!uniform.name.compare(ShaderProgram::uniProjectionMatName)){
            projectionMatrixUniLoc = uniform.location;
        }else if(!uniform.name.compare(ShaderProgram::uniNormalMName)){
            normMatUniLoc = uniform.location;
        }else if(!uniform.name.compare("uLight.position")){
            lightPosUniLoc = uniform.location;
        }else if(!uniform.name.compare("uLight.color")){
            lightColorUniLoc = uniform.location;
        }
    }
}

void GreySP::setUniforms(ObjMaterial *mat){
    NormalSP::setUniforms(mat);
    m4d matrix = Camera::instance()->normalMatrix();
    glUniformMatrix4fv(normMatUniLoc, 1, GL_TRUE, matrix.pointer());
    sp<LightSource> light = Illuminator::instance()->getLightSource();
    v3d lightPosInEyeSpace = light->getDirectionInEyeSpace();
    glUniform3fv(lightPosUniLoc, 1, &lightPosInEyeSpace.x);
    v4d lightColor = light->getColor();
    glUniform4fv(lightColorUniLoc, 1, &lightColor.x);
}