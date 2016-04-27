#include "SimpleGouraudPhongSP.hpp"


void SimpleGouraudPhongSP::initUniformLocations(){
    initTransformUniformLocations();
    initMaterialUniformLocations();
    initLightUniLocations();
}

void SimpleGouraudPhongSP::setUniforms(const ObjMaterial *mat){
    setTransformUniforms();
    setLightUniforms();
    setMaterialUniforms(mat);
}

#pragma mark Helpers
void SimpleGouraudPhongSP::initLightUniLocations(){
    lightLoc.type       = getUniformLocation("uLight.type"      );
    lightLoc.position   = getUniformLocation("uLight.position"  );
    lightLoc.direction  = getUniformLocation("uLight.direction" );
    lightLoc.color      = getUniformLocation("uLight.color"     );
}


void SimpleGouraudPhongSP::setLightUniforms(){
    sp<LightSource> light = Illuminator::instance()->getLightSource();
    
    glUniform1i(lightLoc.type, light->type());
    
    v3d temp = light->getPositionInEyeSpace();
    glUniform3fv(lightLoc.position, 1, &temp.x);
    temp = light->getDirectionInEyeSpace();
    glUniform3fv(lightLoc.direction, 1, &temp.x);
    
    glUniform4fv(lightLoc.color,1, &light->getColor().x);
}

void SimpleGouraudPhongSP::setMaterialUniforms(const ObjMaterial *mat){
    glUniform4fv(matLoc.ambient, 1, &mat->ambient.x);
    glUniform4fv(matLoc.diffuse, 1, &mat->diffuse.x);
    glUniform4fv(matLoc.specular, 1, &mat->specular.x);
    
    glUniform1f(matLoc.shininess, mat->specularExponent);
    
    glUniform1i(texLoc.diffuse, 1);
    
    if(texLoc.bump > 0)
        glUniform1i(texLoc.bump, 4);
}
