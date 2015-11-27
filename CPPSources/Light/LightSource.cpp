#include "LightSource.hpp"

#include "OMUtils.h"


LightSource::LightSource() : mPosition(v4d(1.0f, 0.0f, 5.0f, 1.0f)){
    logMessage("LightSource constructor!\n");
}

LightSource::LightSource(v4d position) : mPosition(position){ }

LightSource::~LightSource(){
    logMessage("Light Source destructor\n");
}