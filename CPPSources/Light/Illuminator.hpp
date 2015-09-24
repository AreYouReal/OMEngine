#pragma once

#include <memory>
#include <vector>
#include "LightSource.hpp"

class Illuminator{
public:
    static std::shared_ptr<Illuminator> mInstance;
    static std::shared_ptr<Illuminator> instance();

    bool AddLightSource(LIGHTSOURCE lSource);
    
    LIGHTSOURCE getLightSource();
    
private:
    
    Illuminator();
    
    std::vector<LIGHTSOURCE> lightSources;
};