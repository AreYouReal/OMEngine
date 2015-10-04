#pragma once

#include <memory>
#include <vector>

#include "Shortcuts.h"

#include "LightSource.hpp"


class Illuminator{
public:
    
    
    Illuminator(const Illuminator& rhs) = delete;
    Illuminator& operator=(const Illuminator& rhs) = delete;
    ~Illuminator();
    

    static Illuminator *instance();

    bool AddLightSource(sp<LightSource> lSource);
    
    LightSource* getLightSource();
    
private:
    
    static Illuminator *mInstance;
    Illuminator();

    
    std::vector<LightSource> lightSources;
};