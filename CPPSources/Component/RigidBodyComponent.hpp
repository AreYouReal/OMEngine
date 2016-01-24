#pragma once

#include "GameObject.hpp"

class RigidBodyComponent : public IComponent{
public:
    RigidBodyComponent(GameObject * const gameObject);
    
    virtual ~RigidBodyComponent();

    
    virtual bool init();
    virtual void update();
    virtual void onDestroy();
    
private:

};