#pragma once


#include <memory>

#include "v3d.h"

#include "Camera.h"

enum LightType{ POINT, DIRECTION };

class LightSource{

public:
    
    LightSource();
    LightSource(v4d position);
    
    const v4d& getPosition() const{ return mPosition; }
    const v4d getPositionInEyeSpace() const{ return (mPosition * Camera::instance()->viewMatrix()); }
    const LightType type() const { return lightType; }
private:
    v4d mPosition;
    LightType lightType;
};

typedef std::shared_ptr<LightSource> LIGHTSOURCE;