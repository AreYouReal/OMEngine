

#include "Illuminator.hpp"
#include "OMUtils.h"

Illuminator::Illuminator(){
    logGLError();
    lightSources.push_back(std::make_shared<LightSource>(LightSource::Type::DIRECTION, v3d(0, 0, 5), v4d(1, 1, 1, 1)));
    lightSources.push_back(std::make_shared<LightSource>(LightSource::Type::POINT, v3d(0, -15, 5), v4d(1, 0, 0, 1), 100));
}

Illuminator::~Illuminator(){
}

sp<LightSource> Illuminator::getLightSource(){
    if(lightSources.size() > 0) return lightSources[0];
    return nullptr;
}

sp<LightSource> Illuminator::getLightSource(int index){
    if(index < 0 || index >= lightSources.size()) return lightSources[0];
    return lightSources[index];
}

bool Illuminator::addLightSource(sp<LightSource> lSource){
    lightSources.push_back(lSource);
    return true;
}


void Illuminator::update(const float deltaTime){
    static float alpha = 0;
    static float height = 10;
    
    if(alpha > 360) alpha = 0;
    
    float rad = PI * alpha/ 180;
    float x = cosf(rad) * height;
    float y = sinf(rad) * height;
    v3d pos(x, y, height);
    lightSources[0]->setPosition(pos);
//    v3d::print(pos);
    alpha += 50.0f * deltaTime;
    
    static bool on = false;
    static unsigned int start = getMilliTime();
    if( getMilliTime() - start >= 500 ) {
        if(on){
            lightSources[1]->setColor(v4d(1, 0, 0, 1));
            on = false;
        }else{
            lightSources[1]->setColor(v4d(0, 0, 0, 1));
            on = true;
        }
        start = getMilliTime();
    }
}

