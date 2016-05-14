#pragma once

#include "GameObject.hpp"
#include "ArrowAction.hpp"

#include <queue>

class ArrowAction;

class PlayerController{
public:
    PlayerController(GameObject * const gameObject);
    ~PlayerController();
    
    void debugInit();
    
    void onTouch();
    
    void update();
    
    std::queue<ArrowAction> actions{};
    std::queue<ArrowAction> currentAction{};
    
private:
    
    float playerSpeed = 10;
    
    btQuaternion rotationCorrection{btVector3(0, 0, 1), 0.5f * PI};
    
// Fields
    GameObject *mGo;
    RigidBodyComponent *mRigidBodyComp;

// Helpers
    void rotate();
    void refreshVelocity();
    
};