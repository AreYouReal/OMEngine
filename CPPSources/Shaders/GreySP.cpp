#include "GreySP.hpp"


void GreySP::initUniformLocations(){
    for(const auto &uniform : uniforms){
        if(!uniform.second.name.compare(ShaderProgram::uniModelViewMatName)){
            modelViewMatrixUniLoc = uniform.second.location;
        }else if(!uniform.second.name.compare(ShaderProgram::uniProjectionMatName)){
            projectionMatrixUniLoc = uniform.second.location;
        }else if(!uniform.second.name.compare(ShaderProgram::uniNormalMName)){
            normMatUniLoc = uniform.second.location;
        }else if(!uniform.second.name.compare("uLight.position")){
            lightPosUniLoc = uniform.second.location;
        }else if(!uniform.second.name.compare("uLight.color")){
            lightColorUniLoc = uniform.second.location;
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