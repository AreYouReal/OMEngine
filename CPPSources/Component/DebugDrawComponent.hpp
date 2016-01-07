#pragma once

#include "IComponent.hpp"
#include "m4d.h"
#include "WiredCube.hpp"

class DebugDrawComponent: public IComponent{
public:
    DebugDrawComponent( GameObject * const gameObject, const v3d size);
    
    virtual ~DebugDrawComponent();
    
    virtual bool init()     override;
    virtual void update()   override;
    virtual void onDestroy()  override;
    
private:
    WiredCube wc;
    m4d mModelMatrix;
};