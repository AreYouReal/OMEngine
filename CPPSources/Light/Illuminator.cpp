

#include "Illuminator.hpp"
#include "OMUtils.h"

Illuminator::Illuminator(){
    lightSources.push_back(std::make_shared<LightSource>(LightSource::Type::POINT, v3d(0, 5, 5), v4d(1, 1, 1, 1)));
    lightSources.push_back(std::make_shared<LightSource>(LightSource::Type::POINT, v3d(0, -5, 5), v4d(1, 0, 0, 1)));
}

Illuminator::~Illuminator(){
}

sp<LightSource> Illuminator::getLightSource(){
    if(lightSources.size() > 0) return lightSources[0];
    return nullptr;
}

sp<LightSource> Illuminator::getLightSource(int index){
    if(index < 0 || index >= lightSources.size()) return lightSources[0];
    return lightSources[index];
}

bool Illuminator::addLightSource(sp<LightSource> lSource){
    lightSources.push_back(lSource);
    return true;
}