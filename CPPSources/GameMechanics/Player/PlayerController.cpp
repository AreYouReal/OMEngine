#include "PlayerController.hpp"
#include "btSphereShape.h"

PlayerController::PlayerController(GameObject * const gameObject){
    mGo = gameObject;
    mRigidBodyComp = static_cast<RigidBodyComponent*>(mGo->getComponent(ComponentEnum::RIGID_BODY));
    mRigidBodyComp->mBody->setAngularFactor(btVector3(0, 0, 0));
    mRigidBodyComp->mBody->setFriction(0.0);
    mRigidBodyComp->mBody->setRestitution(0.0);
//    btTransform t = mRigidBodyComp->mBody->getWorldTransform();
//    btQuaternion currQ = t.getRotation();
//    currQ.setEuler(0, 0, PI);
//    t.setRotation(currQ);
//    mRigidBodyComp->mBody->setWorldTransform(t);
    mGo->mTransform.mScale = v3d(0.5, 0.5, 0.5);
    
    mRigidBodyComp->mBody->setGravity(btVector3(0, 0, -98));
    
    AnimMeshComponent *amc = static_cast<AnimMeshComponent *>(mGo->getComponent(ComponentEnum::ANIM_MESH));
    if(amc){
//        amc->md5->
    }

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
    mRigidBodyComp->mBody->activate();
    rotate();
    refreshVelocity();
}

void PlayerController::rotate(){
    if(mRigidBodyComp){
        btTransform t = mRigidBodyComp->mBody->getWorldTransform();
        btQuaternion currQ = t.getRotation();
//        logMessage("Quat(before): %f, %f, %f, %f\n", currQ.x(), currQ.y(), currQ.z(), currQ.w() );

        ArrowAction act;
        if(currentAction.size() > 0){
            act = currentAction.front();
            currentAction.pop();
        }
        btQuaternion q(act.rotation.x, act.rotation.y, act.rotation.z, act.rotation.w);
        currQ = q;
//        logMessage("Quat(after): %f, %f, %f, %f\n", currQ.x(), currQ.y(), currQ.z(), currQ.w() );
        t.setRotation(currQ);
        mRigidBodyComp->mBody->setWorldTransform(t);
        mGo->mTransform.rotate(act.rotation);
    }
}

void PlayerController::refreshVelocity(){
    v3d front = mGo->mTransform.mFront.normalize();
    mRigidBodyComp->mBody->setLinearVelocity(btVector3(front.x * playerSpeed, front.y * playerSpeed, front.z * playerSpeed));
}


void PlayerController::update(){
    v3d pos = mGo->getPosition();
    if(pos.z < -10){
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