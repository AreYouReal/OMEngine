#include "PlayerController.hpp"


PlayerController::PlayerController(GameObject * const gameObject){
    mGo = gameObject;
    mRigidBodyComp = static_cast<RigidBodyComponent*>(mGo->getComponent(ComponentEnum::RIGID_BODY));
    mRigidBodyComp->mBody->setAngularFactor(btVector3(0, 0, 0));
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
        if(actions.size() > 0){
            angle = actions.front();
            logMessage("ANGLE: %f\n", angle);
            actions.pop();
        }
        q.setRotation(btVector3(0, 0, 1), angle);
        currQ *= q;
//        logMessage("Quat(after): %f, %f, %f, %f\n", currQ.x(), currQ.y(), currQ.z(), currQ.w() );
        t.setRotation(currQ);
        mRigidBodyComp->mBody->setWorldTransform(t);
        mGo->mTransform.rotate(90, v3d(0, 0, 1));
    }
}

void PlayerController::refreshVelocity(){
    v3d front = mGo->mTransform.mFront.normalize();
    mRigidBodyComp->mBody->setLinearVelocity(btVector3(front.x * playerSpeed, front.y * playerSpeed, front.z * playerSpeed));
}


void PlayerController::update(){
    if(mGo->getPosition().z < -10){
        btTransform t = mRigidBodyComp->mBody->getWorldTransform();
        t.setOrigin(btVector3(0, 0, 10));
        btQuaternion q;
        q.setEuler(0, 0, 0);
        t.setRotation(q);
        mRigidBodyComp->mBody->setWorldTransform(t);
        mRigidBodyComp->mBody->setLinearVelocity(btVector3(0, 0, 0));
        

    }
        
}

PlayerController::~PlayerController(){

}