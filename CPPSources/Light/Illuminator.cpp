

#include "Illuminator.hpp"
#include "OMUtils.h"

Illuminator::Illuminator(){
    lightSources.push_back(std::make_shared<LightSource>(LightSource::Type::POINT, v3d(10, 10, 3), v4d(1, 1, 1, 1)));
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