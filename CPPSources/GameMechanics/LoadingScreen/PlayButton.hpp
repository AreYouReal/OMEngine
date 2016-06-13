#pragma once

#include "IComponent.hpp"
#include "Shortcuts.h"
#include "RigidBodyComponent.hpp"

class PlayButton : public IComponent{
public:
    
    PlayButton(GameObject * const gameObject);
    ~PlayButton(){};
    
    virtual void update() override;
    
    static up<GameObject> create();

    
    RigidBodyComponent *mRigidBodyComp;
};