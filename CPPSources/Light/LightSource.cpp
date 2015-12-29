#include "LightSource.hpp"

#include "OMUtils.h"


LightSource::LightSource() : mPosition(-.3f, -.3f, -.9f, 1.0f), mColor(1.0f, 1.0f, 1.0f, 1.0f){
    logMessage("LightSource constructor!\n");
}

LightSource::LightSource(v4d position) : mPosition(position){ }

LightSource::~LightSource(){
    logMessage("Light Source destructor\n");
}