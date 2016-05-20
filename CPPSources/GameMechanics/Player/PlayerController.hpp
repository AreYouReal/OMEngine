#pragma once

#include "IComponent.hpp"
#include "ArrowAction.hpp"
#include "RigidBodyComponent.hpp"
#include "LevelBuilder.hpp"

#include <queue>

class ArrowAction;

class PlayerController : IComponent{
public:
    PlayerController(GameObject * const gameObject);
    virtual ~PlayerController();
    
    virtual void update() override;
    
    void init(LevelBuilder *lb);
    void onTouch();
    
private:
    
    v3d frontVector{0, 0, 1};
    v3d currentFronVector{};
    
    float playerSpeed = 10;
    
    btQuaternion rotationCorrection{btVector3(0, 1, 0), 0.5f * PI};
    
// Fields
    RigidBodyComponent *mRigidBodyComp;

    LevelBuilder    *mLevelBuilder;
    
// Helpers
    void rotate();
    void refreshVelocity();
};