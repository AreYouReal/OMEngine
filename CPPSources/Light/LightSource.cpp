#include "LightSource.hpp"


LightSource::LightSource() : mPosition(v4d(0.0f, -3.0f, -4.0f, 1.0f)){}

LightSource::LightSource(v4d position) : mPosition(position){ }
