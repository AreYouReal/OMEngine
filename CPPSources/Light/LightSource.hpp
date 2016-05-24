#pragma once

#include <memory>
#include "ObjMaterial.h"
#include "IComponent.hpp"
#include "GameObject.hpp"

class LightSource : public IComponent{

public:
    enum Type{ DIRECTION = 0, POINT = 1, SPOT = 2 };
  
    // IComponent
    LightSource(GameObject * const);
    LightSource(GameObject * const, Type type, v4d color, float distance = 30, float linAttenuation = .5f, float quadAttenuation = 1.0f);
    virtual ~LightSource();
   
    virtual void update() override;
    //--------
    
    const v4d&  getColor()      const { return mColor; }
    const v3d  getPosition()   const { return go->getPosition(); }
    const Type  type()          const { return mType; }
    const float distance()      const { return mDistance; }
    const float linearAtten()   const { return mLinearAttenuation; }
    const float quadAtten()     const {return mQuadraticAttenuation; }
    const v4d   getPositionInEyeSpace()  const;
    const v3d   getDirectionInEyeSpace() const;

    void setColor(v4d color);
    
    m4d     getLookAtFromPointView();
    
    float spotCosCutoff = 0.0f;
    float spotBlend = 0.05f;
    
private:
    Type    mType;
    v4d     mColor;
    float   mLinearAttenuation;
    float   mQuadraticAttenuation;
    float   mDistance;
    
    unsigned int vbo;
    unsigned int vao;
    
    void setAttributes() const;
};