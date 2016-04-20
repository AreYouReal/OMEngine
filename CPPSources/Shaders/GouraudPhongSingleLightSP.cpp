#include "GouraudPhongSingleLightSP.hpp"


void GouraudPhongSingleLightSP::initAttribLocations(){
    for(const auto &attrib : attribArray){
        if(!attrib.name.compare(posAttribName)){
            attribLocations[Attributes::POSITION] = attrib.location;
        }else if(!attrib.name.compare(normAttribName)){
            attribLocations[Attributes::NORMAL] = attrib.location;
        }else if(!attrib.name.compare(texCoordAttribName)){
            attribLocations[Attributes::TEXTURE] = attrib.location;
        }
    }
}

void GouraudPhongSingleLightSP::initUniformLocations(){
    for(const auto &uniform : uniformArray){
        if(!uniform.name.compare("uLight.position")){
            lightPositionUniLoc = uniform.location;
        }else if(!uniform.name.compare("uLight.type")){
            lightTypeUniLoc = uniform.location;
        }else if(!uniform.name.compare("uLight.direction")){
            lightDirUniLoc = uniform.location;
        }else if(!uniform.name.compare("uLight.color")){
            lightColorUniLoc = uniform.location;
        }else if(!uniform.name.compare("uMaterial.ambient")){
            materialAmbientUniLoc = uniform.location;
        }else if(!uniform.name.compare("uMaterial.diffuse")){
            materialDiffuseUniLoc = uniform.location;
        }else if(!uniform.name.compare("uMaterial.specular")){
            materialSpecularUniLoc = uniform.location;
        }else if(!uniform.name.compare("uMaterial.shininess")){
            materialShininessUniLoc = uniform.location;
        }else if(!uniform.name.compare("uMaterial.dissolve")){
            materialDissolveUniLoc = uniform.location;
        }else if(!uniform.name.compare(ShaderProgram::uniModelViewMatName)){
            modelViewMatUniLoc = uniform.location;
        }else if(!uniform.name.compare(ShaderProgram::uniProjectionMatName)){
            projectionMatUniLoc = uniform.location;
        }else if(!uniform.name.compare(ShaderProgram::uniNormalMName)){
            normalMatUniLoc = uniform.location;
        }else if(!uniform.name.compare("uSamplerDiffuse")){
            diffuseSamplerUniLoc = uniform.location;
        }
    }
}

void GouraudPhongSingleLightSP::setUniforms(ObjMaterial *mat){
    m4d matrix = Camera::instance()->modelViewMatrix();
    glUniformMatrix4fv(modelViewMatUniLoc, 1, GL_TRUE, matrix.pointer());
    matrix = Camera::instance()->projectionMatrix();
    glUniformMatrix4fv(projectionMatUniLoc, 1, GL_TRUE, matrix.pointer());
    matrix = Camera::instance()->normalMatrix();
    glUniformMatrix4fv(normalMatUniLoc, 1, GL_TRUE, matrix.pointer());
    sp<LightSource> light = Illuminator::instance()->getLightSource();
    v4d lightPosInEyeSpace = light->getPositionInEyeSpace();
    glUniform4fv(lightPositionUniLoc, 1, &lightPosInEyeSpace.x);
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
}