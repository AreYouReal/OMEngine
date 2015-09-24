#include "LightSource.hpp"


LightSource::LightSource() : mPosition(v4d(1.0f, 30.0f, 1.0f, 1.0f)){}

LightSource::LightSource(v4d position) : mPosition(position){ }
