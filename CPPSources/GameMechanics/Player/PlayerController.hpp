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
    
// Fields
    GameObject *mGo;
    RigidBodyComponent *mRigidBodyComp;

// Helpers
    void rotate();
    void refreshVelocity();
    
};