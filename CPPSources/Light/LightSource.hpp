#pragma once


#include <memory>

#include "v3d.h"

#include "Camera.h"

#include "ObjMaterial.h"

#include "WiredCube.hpp"


class LightSource{

public:
    enum Type{ DIRECTION = 0, POINT = 1 };
    
    LightSource();
    LightSource(Type type, v3d position, v4d color);
    ~LightSource();
    
    const v4d&  getColor() const { return mColor; }
    const v3d&  getPosition() const{ return mTransform.mPosition; }
    const v4d   getPositionInEyeSpace() const;
    const Type  type() const { return mType; }
    
    
    void draw() const;
    
private:
    Type    mType;
    v4d     mColor;
    
    Transform mTransform;
    
    unsigned int vbo;
    unsigned int vao;
    sp<ObjMaterial> mMaterial;
    
    WiredCube wc;
    
    void setAttributes() const;
};