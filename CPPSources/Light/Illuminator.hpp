#pragma once

#include <memory>
#include <vector>

#include "Singleton.hpp"

#include "Shortcuts.h"
#include "LightSource.hpp"


class Illuminator : public Singleton<Illuminator>{
public:
        Illuminator();
    ~Illuminator();

    bool AddLightSource(sp<LightSource> lSource);
    
    LightSource* getLightSource();
    
private:
  
    std::vector<LightSource> lightSources;
};