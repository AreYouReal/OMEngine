#pragma once


#include <memory>

#include "v3d.h"

#include "Camera.h"

#include "ObjMaterial.h"

enum LightType{ POINT, DIRECTION };

class LightSource{

public:
    
    LightSource();
    LightSource(v4d position);
    ~LightSource();
    
    const v4d& getColor() const { return mColor; }
    const v4d& getPosition() const{ return mPosition; }
    const v4d getPositionInEyeSpace() const{ return (mPosition * Camera::instance()->viewMatrix()); }
    const LightType type() const { return lightType; }
    
    
    void draw() const;
    
private:
    v4d mPosition;
    LightType lightType;
    v4d     mColor;
    
    
    unsigned int vbo;
    unsigned int vao;
    sp<ObjMaterial> mMaterial;
    
    
    void setAttributes() const;
};