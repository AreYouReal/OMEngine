#include "LightSource.hpp"

#include "OMUtils.h"

LightSource::LightSource() : mTransform(v3d(-1.0f, -.3f, .1f)), mColor(1.0f, 1.0f, 1.0f, 1.0f){
    logMessage("LightSource constructor!\n");
}

LightSource::LightSource(v4d position){
    mTransform.mPosition = position;
}

LightSource::~LightSource(){
    logMessage("Light Source destructor\n");
}

void LightSource::draw() const{
    Camera::instance()->pushMMatrix(mTransform.transformMatrix());
    wc.draw();
    Camera::instance()->popMMatrix();
}