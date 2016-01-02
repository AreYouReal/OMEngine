#include "LightSource.hpp"

#include "OMUtils.h"

static v3d cubeVertices[] = {
    v3d(-.5f, -.5f, .5f),   // 0
    v3d(.5f, -.5f, .5f),    // 1
    v3d(.5f, .5f, .5f),     // 2
    v3d(-.5f, .5f, .5f),    // 3
    v3d(-.5f, .5f, -.5f),   // 4
    v3d(.5f, .5f, -.5f),    // 5
    v3d(.5f, -.5f, -.5f),   // 6
    v3d(-.5f, -.5f, -.5f)   // 7
};


LightSource::LightSource() : mTransform(v3d(0.0f, 0.0f, 9.0f)), mColor(1.0f, 1.0f, 1.0f, 1.0f){
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