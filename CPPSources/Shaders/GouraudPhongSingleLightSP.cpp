#include "GouraudPhongSingleLightSP.hpp"

void GouraudPhongSingleLightSP::initUniformLocations(){
    for(const auto &uniform : uniforms){
        if(!uniform.second.name.compare("uLight.position")){
            lightPositionUniLoc = uniform.second.location;
        }else if(!uniform.second.name.compare("uLight.type")){
            lightTypeUniLoc = uniform.second.location;
        }else if(!uniform.second.name.compare("uLight.direction")){
            lightDirUniLoc = uniform.second.location;
        }else if(!uniform.second.name.compare("uLight.color")){
            lightColorUniLoc = uniform.second.location;
        }else if(!uniform.second.name.compare("uMaterial.ambient")){
            materialAmbientUniLoc = uniform.second.location;
        }else if(!uniform.second.name.compare("uMaterial.diffuse")){
            materialDiffuseUniLoc = uniform.second.location;
        }else if(!uniform.second.name.compare("uMaterial.specular")){
            materialSpecularUniLoc = uniform.second.location;
        }else if(!uniform.second.name.compare("uMaterial.shininess")){
            materialShininessUniLoc = uniform.second.location;
        }else if(!uniform.second.name.compare("uMaterial.dissolve")){
            materialDissolveUniLoc = uniform.second.location;
        }else if(!uniform.second.name.compare(ShaderProgram::uniModelViewMatName)){
            modelViewMatUniLoc = uniform.second.location;
        }else if(!uniform.second.name.compare(ShaderProgram::uniProjectionMatName)){
            projectionMatUniLoc = uniform.second.location;
        }else if(!uniform.second.name.compare(ShaderProgram::uniNormalMName)){
            normalMatUniLoc = uniform.second.location;
        }else if(!uniform.second.name.compare(ShaderProgram::uniShadowMName)){
            shadowMatUniLoc = uniform.second.location;
        }else if(!uniform.second.name.compare("uSamplerDiffuse")){
            diffuseSamplerUniLoc = uniform.second.location;
        }else if(!uniform.second.name.compare("uSamplerBump")){
            bumpSamplerUniLoc = uniform.second.location;
        }else if(!uniform.second.name.compare("uSamplerAmbient")){
            ambientSamplerDiffuse = uniform.second.location;
        }
    }
}

void GouraudPhongSingleLightSP::setUniforms(const ObjMaterial *mat){
    m4d matrix = Camera::instance()->modelViewMatrix();
    glUniformMatrix4fv(modelViewMatUniLoc, 1, GL_TRUE, matrix.pointer());
    matrix = Camera::instance()->projectionMatrix();
    glUniformMatrix4fv(projectionMatUniLoc, 1, GL_TRUE, matrix.pointer());
    matrix = Camera::instance()->normalMatrix();
    glUniformMatrix4fv(normalMatUniLoc, 1, GL_TRUE, matrix.pointer());
    matrix = Camera::instance()->projectorMatrix();
    glUniformMatrix4fv(shadowMatUniLoc, 1, GL_TRUE, matrix.pointer());
    sp<LightSource> light = Illuminator::instance()->getLightSource();
    v4d lightPosInEyeSpace = light->getPositionInEyeSpace();
    glUniform3fv(lightPositionUniLoc, 1, &lightPosInEyeSpace.x);
    v4d lightDirInEyeSpace = light->getDirectionInEyeSpace();
    glUniform3fv(lightDirUniLoc, 1, &lightDirInEyeSpace.x);
    v4d lightColor = light->getColor();
    glUniform4fv(lightColorUniLoc, 1, &lightColor.x);
    glUniform4fv(materialAmbientUniLoc, 1, &mat->ambient.x);
    glUniform4fv(materialDiffuseUniLoc, 1, &mat->diffuse.x);
    glUniform4fv(materialSpecularUniLoc, 1, &mat->specular.x);
    glUniform1f(materialShininessUniLoc, mat->specularExponent);
    glUniform1f(materialDissolveUniLoc, mat->dissolve);
    glUniform1i(diffuseSamplerUniLoc, 1);
    glUniform1i(bumpSamplerUniLoc, 4);
}