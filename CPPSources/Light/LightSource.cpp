#include "LightSource.hpp"

#include "OMUtils.h"
#include <cmath>

LightSource::LightSource() : LightSource(Type::SPOT, v3d(), v4d()){
    logMessage("LightSource constructor!\n");

}

LightSource::LightSource(Type type, v3d position, v4d color, float distance, float linAttenuation, float quadAttenuation): mType(type), mTransform(position), mColor(color), mLinearAttenuation(linAttenuation), mQuadraticAttenuation(quadAttenuation), mDistance(distance){
    spotCosCutoff = cosf(M_PI/180 * 35.0f);
    mTransform.mFront = mTransform.mPosition.normalize();
}

LightSource::~LightSource(){
    logMessage("Light Source destructor\n");
}

void LightSource::draw() const{
    Camera::instance()->pushMMatrix(mTransform.transformMatrix() * m4d::scale(distance(), distance(), distance()));
    wc.draw();
    Camera::instance()->popMMatrix();
}


const v4d LightSource::getPositionInEyeSpace() const{
    v3d position = (Camera::instance()->viewMatrix() * v4d(mTransform.mPosition, 1.0));
    return position;
}

const v3d LightSource::getDirectionInEyeSpace() const{
    v3d direction =  (Camera::instance()->viewMatrix() * mTransform.mFront);
    return direction;
}