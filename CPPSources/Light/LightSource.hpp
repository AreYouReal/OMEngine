#pragma once


#include <memory>

#include "v3d.h"

#include "Camera.h"

#include "ObjMaterial.h"

#include "WiredCube.hpp"


class LightSource{

public:
    enum Type{ DIRECTION = 0, POINT = 1, SPOT = 2 };
    
    LightSource();
    LightSource(Type type, v3d position, v4d color, float distance = 10, float linAttenuation = 0.5f, float quadAttenuation = 1.0f);
    ~LightSource();
    
    const v4d&  getColor()      const { return mColor; }
    const v3d&  getPosition()   const { return mTransform.mPosition; }
    const Type  type()          const { return mType; }
    const float distance()      const { return mDistance; }
    const float linearAtten()   const { return mLinearAttenuation; }
    const float quadAtten()     const {return mQuadraticAttenuation; }
    const v4d   getPositionInEyeSpace()  const;
    const v3d   getDirectionInEyeSpace() const;
    
    void draw() const;
    
    void setPosition(const v3d pos);
    
    m4d     getLookAtFromPointView();
    
    float spotCosCutoff = 0.0f;
    float spotBlend = 0.05f;
    
private:
    Type    mType;
    v4d     mColor;
    float   mLinearAttenuation;
    float   mQuadraticAttenuation;
    float   mDistance;
    
    Transform mTransform;
    
    unsigned int vbo;
    unsigned int vao;
    
    
    // Separate debug draw
    sp<ObjMaterial> mMaterial;
    WiredCube wc;
    //----------------
    
    void setAttributes() const;
};