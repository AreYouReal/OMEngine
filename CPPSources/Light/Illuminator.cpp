

#include "Illuminator.hpp"
#include "OMUtils.h"

Illuminator::Illuminator(){
    lightSources.push_back(std::make_shared<LightSource>(LightSource::Type::SPOT, v3d(0, 0, 5), v4d(1, 1, 1, 1)));
    logMessage("Illuminator constructor!\n");
}

Illuminator::~Illuminator(){
    logMessage("Illuminator destructor\n");
}

sp<LightSource> Illuminator::getLightSource(){
    if(lightSources.size() > 0) return lightSources[0];
    return nullptr;
}

bool Illuminator::addLightSource(sp<LightSource> lSource){
    lightSources.push_back(lSource);
    return true;
}