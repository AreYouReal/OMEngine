#include "PlayerController.hpp"
#include "btSphereShape.h"

PlayerController::PlayerController(GameObject * const gameObject){
    mGo = gameObject;
    mRigidBodyComp = static_cast<RigidBodyComponent*>(mGo->getComponent(ComponentEnum::RIGID_BODY));
    mRigidBodyComp->mBody->setAngularFactor(btVector3(0, 0, 0));
    mRigidBodyComp->mBody->setFriction(0.0);
    mRigidBodyComp->mBody->setRestitution(0.0);
    mGo->mTransform.mScale = v3d(0.5, 0.5, 0.5);
    
    mRigidBodyComp->mBody->setGravity(btVector3(0, 0, -98));
    
    AnimMeshComponent *amc = static_cast<AnimMeshComponent *>(mGo->getComponent(ComponentEnum::ANIM_MESH));
    if(amc){
//        amc->md5->
    }
    
    // Figure out why box shape penetrates in bulding blocks....
    {
        delete mRigidBodyComp->mBody->getCollisionShape();
        btSphereShape *newShaper = new btSphereShape(1.0f);
        mRigidBodyComp->mBody->setCollisionShape(newShaper);
    }
}

void PlayerController::debugInit(){
    currentAction = actions;
}

void PlayerController::onTouch(){
    rotate();
    refreshVelocity();
}

void PlayerController::rotate(){
    if(mRigidBodyComp){
        btTransform t = mRigidBodyComp->mBody->getWorldTransform();
        btQuaternion currQ = t.getRotation();
//        logMessage("Quat(before): %f, %f, %f, %f\n", currQ.x(), currQ.y(), currQ.z(), currQ.w() );
        btQuaternion q;
        float angle = 0.0f;
        if(currentAction.size() > 0){
            angle = currentAction.front();
            logMessage("ANGLE: %f\n", angle);
            currentAction.pop();
        }
        q.setRotation(btVector3(0, 0, 1), angle + PI * 0.5);
        currQ = q;
//        logMessage("Quat(after): %f, %f, %f, %f\n", currQ.x(), currQ.y(), currQ.z(), currQ.w() );
        t.setRotation(currQ);
        mRigidBodyComp->mBody->setWorldTransform(t);
        mGo->mTransform.rotate(180 / PI * angle, v3d(0, 0, 1));
    }
}

void PlayerController::refreshVelocity(){
    v3d front = mGo->mTransform.mFront.normalize();
    mRigidBodyComp->mBody->setLinearVelocity(btVector3(front.x * playerSpeed, front.y * playerSpeed, front.z * playerSpeed));
}


void PlayerController::update(){
    if(mGo->getPosition().z < -10){
        btTransform t = mRigidBodyComp->mBody->getWorldTransform();
        t.setOrigin(btVector3(0, 0, 1));
        btQuaternion q;
        q.setEuler(0, 0, 0);
        t.setRotation(q);
        mRigidBodyComp->mBody->setWorldTransform(t);
        mRigidBodyComp->mBody->setLinearVelocity(btVector3(0, 0, 0));
        currentAction = actions;
    }
        
}

PlayerController::~PlayerController(){

}