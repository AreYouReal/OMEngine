#include "LightSource.hpp"

#include "OMUtils.h"
#include <cmath>
#include "Illuminator.hpp"

LightSource::LightSource(GameObject * const gameObject) : IComponent(gameObject){
    Illuminator::instance()->addLight(this);
}

LightSource::LightSource(GameObject * const gameObject, Type type, v4d color, float distance, float linAttenuation, float quadAttenuation) : IComponent(gameObject), mType(type), mColor(color), mDistance(distance), mLinearAttenuation(linAttenuation), mQuadraticAttenuation(quadAttenuation){
    spotCosCutoff = cosf(M_PI/180 * 55.0f);
    Illuminator::instance()->addLight(this);
}

LightSource::~LightSource(){
    Illuminator::instance()->removeLight(this);
}

const v4d LightSource::getPositionInEyeSpace() const{
    v3d position = (Camera::instance()->viewMatrix() * v4d(go->getPosition(), 1.0));
    return position;
}

const v3d LightSource::getDirectionInEyeSpace() const{
    v3d direction =  (Camera::instance()->viewMatrix() * v3d::normalize(go->getFront()));
    return direction;
}

void LightSource::setColor(v4d color){
    mColor = color;
}

void LightSource::follow(GameObject *go){
    mFollowObj = go;
}

m4d LightSource::getLookAtFromPointView(){
    v3d pos = getPosition();
    v3d up(0, 0, 1);
    v3d center(0, 0, 0);
    return m4d::lookAt(pos, center, up);
}

void LightSource::update(){
    
    if(mFollowObj){
        go->setPosition(mFollowObj->getPosition() + v3d(1, 1, 1));
    }
    
//    static float degrees = 0.0f;
//    if(degrees > 360.0f){
//        degrees = 0.0f;
//    }
//    degrees += 50 * Time::deltaTime;
//    
//    go->mTransform.rotate(degrees, v3d(1, 1, 0));
}