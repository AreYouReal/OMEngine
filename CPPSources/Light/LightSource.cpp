#include "LightSource.hpp"

#include "OMUtils.h"
#include <cmath>

LightSource::LightSource() : LightSource(Type::SPOT, v3d(), v4d()){
    logGLError();
}

LightSource::LightSource(Type type, v3d position, v4d color, float distance, float linAttenuation, float quadAttenuation): mType(type), mTransform(position), mColor(color), mLinearAttenuation(linAttenuation), mQuadraticAttenuation(quadAttenuation), mDistance(distance){
    logGLError();
    spotCosCutoff = cosf(M_PI/180 * 35.0f);
    mTransform.mFront = mTransform.mPosition.normalize();
    logGLError();
}

LightSource::~LightSource(){}

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
    v3d direction =  (Camera::instance()->viewMatrix() * v3d::normalize(mTransform.mPosition));
    return direction;
}

void LightSource::setPosition(const v3d pos){
    mTransform.mPosition = pos;
    mTransform.refreshTransformMatrix();
}

m4d LightSource::getLookAtFromPointView(){
    v3d pos = getPosition();
    v3d up(0, 0, 1);
    v3d center(0, 0, 0);
    return m4d::lookAt(pos, center, up);
}