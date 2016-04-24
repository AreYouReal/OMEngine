#include "GPMultiLightsSP.hpp"


void GPMultiLightsSP::initUniformLocations(){
    for(auto const &uniform : uniforms){
        logMessage("Uniform: %s\n",uniform.second.name.c_str());
    }
}

void GPMultiLightsSP::setUniforms(ObjMaterial *mat){

}