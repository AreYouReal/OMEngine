

#include "Illuminator.hpp"
#include "OMUtils.h"

Illuminator::Illuminator(){
    logGLError();
}

Illuminator::~Illuminator(){

}

LightSource* Illuminator::getLightSource(){
    if(lightSources.size() > 0) return lightSources[0];
    return nullptr;
}

LightSource* Illuminator::getLightSource(int index){
    if(lightSources.empty()) return nullptr;
    if(index < 0 || index >= lightSources.size()) return lightSources[0];
    return lightSources[index];
}

bool Illuminator::addLightSource(LightSource* lSource){
    lightSources.push_back(lSource);
    return true;
}

void Illuminator::addLight(LightSource *light){
    lightSources.push_back(light);
}

void Illuminator::removeLight(LightSource *light){
    for(int i = 0; i < lightSources.size(); ++i){
        if(lightSources[i] == light){
            lightSources.erase(lightSources.begin() + i);
            break;
        }
    }
}