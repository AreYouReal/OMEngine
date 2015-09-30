#pragma once

#include <memory>
#include <vector>
#include "LightSource.hpp"

class Illuminator{
public:
    ~Illuminator();
    

    static Illuminator *instance();

    bool AddLightSource(LIGHTSOURCE lSource);
    
    LIGHTSOURCE getLightSource();
    
private:
    
    static Illuminator *mInstance;
    Illuminator();

    
    std::vector<LIGHTSOURCE> lightSources;
};