#include "LightSource.hpp"

#include "OMUtils.h"


LightSource::LightSource() : mPosition(v4d(-.3f, -.3f, -.9f, 1.0f)){
    logMessage("LightSource constructor!\n");
}

LightSource::LightSource(v4d position) : mPosition(position){ }

LightSource::~LightSource(){
    logMessage("Light Source destructor\n");
}