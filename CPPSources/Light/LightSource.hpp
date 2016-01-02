#pragma once


#include <memory>

#include "v3d.h"

#include "Camera.h"

#include "ObjMaterial.h"

#include "WiredCube.hpp"

enum LightType{ POINT, DIRECTION };

class LightSource{

public:
    
    LightSource();
    LightSource(v4d position);
    ~LightSource();
    
    const v4d& getColor() const { return mColor; }
    const v4d& getPosition() const{ return mTransform.mPosition; }
    const v4d getPositionInEyeSpace() const{ return (mTransform.mPosition * Camera::instance()->viewMatrix()); }
    const LightType type() const { return lightType; }
    
    
    void draw() const;
    
private:
    LightType lightType;
    v4d     mColor;
    
    Transform mTransform;
    
    unsigned int vbo;
    unsigned int vao;
    sp<ObjMaterial> mMaterial;
    
    WiredCube wc;
    
    void setAttributes() const;
};