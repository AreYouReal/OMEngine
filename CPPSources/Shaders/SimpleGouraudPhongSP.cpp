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
    LightSource* light = Illuminator::instance()->getLightSource();

    glUniform1i(lightLoc.type, light->type());
    
    v3d temp = light->getPositionInEyeSpace();
    glUniform3fv(lightLoc.position, 1, &temp.x);
    temp = light->getDirectionInEyeSpace();
    glUniform3fv(lightLoc.direction, 1, &temp.x);
    
    glUniform4fv(lightLoc.color,1, &light->getColor().x);
}


