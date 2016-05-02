#include "PlayerController.hpp"


PlayerController::PlayerController(GameObject * const gameObject){
    mGo = gameObject;
    mRigidBodyComp = static_cast<RigidBodyComponent*>(mGo->getComponent(ComponentEnum::RIGID_BODY));
}

void PlayerController::onTouch(){
    rotate();
    refreshVelocity();
}

void PlayerController::rotate(){
    if(mRigidBodyComp){
        btTransform t = mRigidBodyComp->mBody->getWorldTransform();
        btQuaternion currQ = t.getRotation();
        logMessage("Quat(before): %f, %f, %f, %f\n", currQ.x(), currQ.y(), currQ.z(), currQ.w() );
        btQuaternion q;
        q.setRotation(btVector3(0, 0, 1), PI * 0.5f);
        currQ *= q;
        logMessage("Quat(after): %f, %f, %f, %f\n", currQ.x(), currQ.y(), currQ.z(), currQ.w() );
        t.setRotation(currQ);
        mRigidBodyComp->mBody->setWorldTransform(t);
        mGo->mTransform.rotate(90, v3d(0, 0, 1));
    }
}

void PlayerController::refreshVelocity(){
    v3d front = mGo->mTransform.mFront.normalize();
    mRigidBodyComp->mBody->setLinearVelocity(btVector3(front.x * playerSpeed, front.y * playerSpeed, front.z * playerSpeed));
}

PlayerController::~PlayerController(){

}