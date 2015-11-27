

#include "Illuminator.hpp"
#include "OMUtils.h"

Illuminator::Illuminator(){
    lightSources.push_back(std::make_shared<LightSource>());
    logMessage("Illuminator constructor!\n");
}

Illuminator::~Illuminator(){
    logMessage("Illuminator destructor\n");
}

sp<LightSource> Illuminator::getLightSource(){
    if(lightSources.size() > 0) return lightSources[0];
    return nullptr;
}