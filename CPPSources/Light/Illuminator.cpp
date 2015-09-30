

#include "Illuminator.hpp"
#include "SRUtils.h"

Illuminator *Illuminator::mInstance = NULL;

Illuminator *Illuminator::instance(){
    if(!mInstance){
        mInstance = new Illuminator();
    }
    return mInstance;
}

Illuminator::Illuminator(){
    lightSources.push_back(LIGHTSOURCE(new LightSource()));
    logMessage("Illuminator constructor!\n");
}

Illuminator::~Illuminator(){
    mInstance = 0;
    logMessage("Illuminator destructor\n");
}

LIGHTSOURCE Illuminator::getLightSource(){
    if(lightSources.size() > 0) return lightSources[0];
    return nullptr;
}