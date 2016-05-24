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

    bool addLightSource(LightSource* lSource);
    
    LightSource* getLightSource();
    LightSource* getLightSource(int index);
    
    void update(const float deltaTime);
    
    void addLight(LightSource *light);
    void removeLight(LightSource * light);
    
private:
  
    std::vector<LightSource*> lightSources;
};