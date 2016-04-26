#include "SimpleGouraudSP.hpp"


void SimpleGouraudSP::initUniformLocations(){
    initTransformUniLocations();
    initLightUniLocations();
    initMaterialUniLocations();
}

void SimpleGouraudSP::setUniforms(const ObjMaterial *mat){
    setTransformUniforms();
    setLightUniforms();
    setMaterialUniforms(mat);
}

#pragma mark Helpers
void SimpleGouraudSP::initTransformUniLocations(){
    modelViewM  = getUniformLocation(ShaderProgram::uniModelViewMatName);
    projectionM = getUniformLocation(ShaderProgram::uniProjectionMatName);
    normalM     = getUniformLocation(ShaderProgram::uniNormalMName);
}

void SimpleGouraudSP::initLightUniLocations(){
    lightType   = getUniformLocation("uLight.type");
    lightPos    = getUniformLocation("uLight.position");
    lightDir    = getUniformLocation("uLight.direction");
    lightColor  = getUniformLocation("uLight.color");
}

void SimpleGouraudSP::initMaterialUniLocations(){
    matAmbientColor     = getUniformLocation("uMaterial.ambient");
    matDiffuseColor     = getUniformLocation("uMaterial.diffuse");
    matSpecularColor    = getUniformLocation("uMaterial.specular");
    matShininess        = getUniformLocation("uMaterial.shininess");
}


void SimpleGouraudSP::setTransformUniforms(){
    glUniformMatrix4fv(modelViewM,  1, GL_TRUE, Camera::instance()->modelViewMatrix().pointer() );
    glUniformMatrix4fv(projectionM, 1, GL_TRUE, Camera::instance()->projectionMatrix().pointer());
    glUniformMatrix4fv(normalM,     1, GL_TRUE, Camera::instance()->normalMatrix().pointer()    );
}

void SimpleGouraudSP::setLightUniforms(){
    sp<LightSource> light = Illuminator::instance()->getLightSource();
    
    glUniform1i(lightType, light->type());
    
    v3d temp = light->getPositionInEyeSpace();
    glUniform3fv(lightPos, 1, &temp.x);
    temp = light->getDirectionInEyeSpace();
    glUniform3fv(lightDir, 1, &temp.x);
    
    glUniform4fv(lightColor,1, &light->getColor().x);
}

void SimpleGouraudSP::setMaterialUniforms(const ObjMaterial *mat){
    glUniform4fv(matAmbientColor, 1, &mat->ambient.x);
    glUniform4fv(matDiffuseColor, 1, &mat->diffuse.x);
    glUniform4fv(matSpecularColor, 1, &mat->specular.x);
    
    glUniform1f(matShininess, mat->specularExponent);
}
