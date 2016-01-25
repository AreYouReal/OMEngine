#pragma once

#include "IComponent.hpp"
#include "m4d.h"
#include "WiredCube.hpp"
#include "GameObject.hpp"

class DebugDrawComponent: public IComponent{
public:
    DebugDrawComponent( GameObject * const gameObject);
    
    virtual ~DebugDrawComponent();
    
    virtual bool init()     override;
    virtual void update()   override;
    virtual void draw()     override;
    virtual void onDestroy()  override;
    
private:
    WiredCube wc;
};