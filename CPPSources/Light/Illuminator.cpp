

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
    lightSources.push_back(LightSource());
    logMessage("Illuminator constructor!\n");
}

Illuminator::~Illuminator(){
    if(mInstance){
        delete  mInstance;
        mInstance = 0;
    }
    logMessage("Illuminator destructor\n");
}

LightSource* Illuminator::getLightSource(){
    if(lightSources.size() > 0) return &lightSources[0];
    return nullptr;
}