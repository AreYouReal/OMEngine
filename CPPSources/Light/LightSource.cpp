#include "LightSource.hpp"

#include "SRUtils.h"


LightSource::LightSource() : mPosition(v4d(1.0f, 3.0f, 20.0f, 1.0f)){
    logMessage("LightSource constructor!\n");
}

LightSource::LightSource(v4d position) : mPosition(position){ }

LightSource::~LightSource(){
    logMessage("Light Source destructor\n");
}