#include "LightSource.hpp"

#include "OMUtils.h"

LightSource::LightSource() : LightSource(Type::DIRECTION, v3d(), v4d()){
    logMessage("LightSource constructor!\n");
}

LightSource::LightSource(Type type, v3d position, v4d color, float distance, float linAttenuation, float quadAttenuation): mType(type), mTransform(position), mColor(color), mLinearAttenuation(linAttenuation), mQuadraticAttenuation(quadAttenuation), mDistance(distance){ }

LightSource::~LightSource(){
    logMessage("Light Source destructor\n");
}

void LightSource::draw() const{
    Camera::instance()->pushMMatrix(mTransform.transformMatrix() * m4d::scale(distance(), distance(), distance()));
    wc.draw();
    Camera::instance()->popMMatrix();
}


const v4d LightSource::getPositionInEyeSpace() const{
    v3d position = (Camera::instance()->modelViewMatrix() * mTransform.mPosition);
    return v4d(position, mType);
}