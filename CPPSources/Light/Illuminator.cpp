

#include "Illuminator.hpp"

std::shared_ptr<Illuminator> Illuminator::mInstance = NULL;

std::shared_ptr<Illuminator> Illuminator::instance(){
    if(!mInstance){
        mInstance = std::shared_ptr<Illuminator>(new Illuminator());
    }
    return mInstance;
}

Illuminator::Illuminator(){
    lightSources.push_back(LIGHTSOURCE(new LightSource()));
}

LIGHTSOURCE Illuminator::getLightSource(){
    if(lightSources.size() > 0) return lightSources[0];
    return nullptr;
}