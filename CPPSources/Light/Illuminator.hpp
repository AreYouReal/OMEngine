#pragma once

#include <memory>
#include <vector>

#include "Singleton.hpp"

#include "Shortcuts.h"
#include "LightSource.hpp"


class Illuminator : public Singleton<Illuminator>{
public:
    Illuminator();
    Illuminator(const Illuminator& rhs) = delete;
    Illuminator& operator=(const Illuminator& rhs) = delete;
    ~Illuminator();

    bool addLightSource(sp<LightSource> lSource);
    
    sp<LightSource> getLightSource();
    
private:
  
    std::vector<sp<LightSource>> lightSources;
};