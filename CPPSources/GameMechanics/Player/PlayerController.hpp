#pragma once

#include "IComponent.hpp"
#include "RigidBodyComponent.hpp"
#include <queue>

class LevelBuilder;

class PlayerController : public IComponent{
public:
    PlayerController(GameObject * const gameObject);
    virtual ~PlayerController();
    
    virtual void update() override;
    
    void init(LevelBuilder *lb);
    void onTouch();
    
private:
    
    v3d frontVector{0, 0, 1};
    v3d currentFronVector{};
    
    float playerSpeed = 7.5;
    
// Fields
    RigidBodyComponent  *mRigidBodyComp;
    AnimMeshComponent   *mAnimMeshComp;
    LevelBuilder        *mLevelBuilder;
    
// Helpers
    void applyAction();
    void refreshVelocity();
    bool mActive{false};
    void startPose();
};