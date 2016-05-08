#include "PhongMultiLightSP.hpp"



void PhongMultiLightSP::initUniformLocations(){
    initTransformUniformLocations();
    initMaterialUniformLocations();
    initLightUniLocations();
}


void PhongMultiLightSP::initLightUniLocations(){
    string vsLightName{"uLightVS"};
    string fsLightName{"uLightFS"};
    
    for(int i = 0; i < numLights; ++i){
        string index = "[" + std::to_string(i) + "].";
        string vsLightNameIndex = vsLightName + index;
        
        LightUniformLocations vsLightUniLocs;
        vsLightUniLocs.type           = getUniformLocation(vsLightNameIndex + ShaderProgram::type );
        vsLightUniLocs.position       = getUniformLocation(vsLightNameIndex + ShaderProgram::position );
        vsLightUniLocs.direction      = getUniformLocation(vsLightNameIndex + ShaderProgram::direction);
        vsLightLoc.push_back(vsLightUniLocs);
        
        
        string fsLightNameIndex = fsLightName + index;
        
        LightUniformLocations fsLightUniLocs;
        fsLightUniLocs.color          = getUniformLocation(fsLightNameIndex + ShaderProgram::color);
        fsLightUniLocs.distance       = getUniformLocation(fsLightNameIndex + ShaderProgram::distance);
        fsLightUniLocs.linAtten       = getUniformLocation(fsLightNameIndex + ShaderProgram::linAtten);
        fsLightUniLocs.quadAtten      = getUniformLocation(fsLightNameIndex + ShaderProgram::quadAtten);
        fsLightUniLocs.spotCosCutoff  = getUniformLocation(fsLightNameIndex + ShaderProgram::cosCutoff);
        fsLightUniLocs.spotBlend      = getUniformLocation(fsLightNameIndex + ShaderProgram::blend);
        fsLightLoc.push_back(fsLightUniLocs);
    }
    
    numLightUniLoc = getUniformLocation(ShaderProgram::numLights);
    
}


void PhongMultiLightSP::setUniforms(const ObjMaterial *mat){

}

void PhongMultiLightSP::setLightUniforms(){

}

void PhongMultiLightSP::setUniformForLightSource(sp<LightSource> light, int index){

}

