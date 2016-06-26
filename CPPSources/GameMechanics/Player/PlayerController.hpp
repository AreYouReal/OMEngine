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
    
    void setLevelBuilder(LevelBuilder *lb);
    void onTouch();
    
    void activate();
    bool active(){return mActive;};
    
    void refreshAnimMeshComp();
    
private:
    
    v3d frontVector{0, 0, 1};
    v3d currentFronVector{};
    float upLinVelocity = 0.0f;
    
    float playerSpeed = 7.5;
    
    bool mJump = false;
    float jumpSpeed = 7.5f;
    
    
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