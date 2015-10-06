

#include "Illuminator.hpp"
#include "SRUtils.h"

Illuminator::Illuminator(){
    lightSources.push_back(LightSource());
    logMessage("Illuminator constructor!\n");
}

Illuminator::~Illuminator(){
    logMessage("Illuminator destructor\n");
}

LightSource* Illuminator::getLightSource(){
    if(lightSources.size() > 0) return &lightSources[0];
    return nullptr;
}