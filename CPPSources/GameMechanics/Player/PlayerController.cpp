#include "PlayerController.hpp"
#include "btSphereShape.h"
#include "Scene.hpp"
#include "LevelBuilder.hpp"
#include "BBlock.hpp"

std::vector<v3d> camFollowPositions{v3d(0, 2, 5), v3d(-5, 2, 0), v3d(-5, 2, 5),};


bool onPlayerPhysicalContact(btManifoldPoint &point, const btCollisionObjectWrapper *obj0, int part0, int index0, const btCollisionObjectWrapper *obj1, int part1, int index1){
    GameObject *go1 = ((GameObject*)((btCollisionObject*)obj1->getCollisionObject())->getUserPointer());
//    logMessage("Onbj1 : %s   <-> Obj2: %s \n", go0->name.c_str(), go1->name.c_str());
    
    BBlock* blockComp = static_cast<BBlock*>(go1->getComponent(ComponentEnum::BBLOCK));
    if(blockComp){
        blockComp->hide();
    }
    
    return true;
}

PlayerController::PlayerController(GameObject * const gameObject) : IComponent(gameObject){
    mRigidBodyComp = static_cast<RigidBodyComponent*>(go->getComponent(ComponentEnum::RIGID_BODY));
    mRigidBodyComp->mBody->setAngularFactor(btVector3(0, 0, 0));
    mRigidBodyComp->mBody->setFriction(0.0);
    mRigidBodyComp->mBody->setRestitution(0.0);

   
    go->setPosition(v3d(0, 10, 0));
    
    mRigidBodyComp->mBody->setGravity(btVector3(0, 0, 0));

    Camera::instance()->follow(go, camFollowPositions[0]);
    
//    {
//        delete mRigidBodyComp->mBody->getCollisionShape();
//        btSphereShape *newShaper = new btSphereShape(1.0f);
//        mRigidBodyComp->mBody->setCollisionShape(newShaper);
//    }
    
    mRigidBodyComp->setContantCallback(onPlayerPhysicalContact);
    
}

PlayerController::~PlayerController(){
    onDestroy();
}

void PlayerController::init(LevelBuilder *lb){
    mLevelBuilder = lb;
}

void PlayerController::onTouch(){
    static int camPos = 0;
    if(camPos > 2) camPos = 0;
        Camera::instance()->follow(go, camFollowPositions[camPos++]);
//    mRigidBodyComp->mBody->activate();
//    rotate();
//    refreshVelocity();
}

void PlayerController::rotate(){
    if(mRigidBodyComp){
        btTransform t = mRigidBodyComp->mBody->getWorldTransform();
        btQuaternion currQ = t.getRotation();
        ArrowAction * act = mLevelBuilder->popAction();
        if(act == nullptr) return;
        
        btQuaternion q(act->mRotation.x, act->mRotation.y, act->mRotation.z, act->mRotation.w);
        currQ = q * rotationCorrection;
        t.setRotation(currQ);
        mRigidBodyComp->mBody->setWorldTransform(t);
        
        currentFronVector = frontVector * act->mRotation.matrix();

        AnimMeshComponent *amc = static_cast<AnimMeshComponent*>(go->getComponent(ComponentEnum::ANIM_MESH));
        
        static AnimMeshComponent::AnimationStates currentState = AnimMeshComponent::AnimationStates::IDLE;
        amc->setState( currentState = (currentState == AnimMeshComponent::AnimationStates::IDLE) ? AnimMeshComponent::AnimationStates::RUN : AnimMeshComponent::AnimationStates::IDLE );
    }
}

void PlayerController::refreshVelocity(){
    mRigidBodyComp->mBody->setLinearVelocity(btVector3(currentFronVector.x * playerSpeed, currentFronVector.y * playerSpeed, currentFronVector.z * playerSpeed));
}


void PlayerController::update(){
    v3d pos = go->getPosition();
    if(pos.y < -10){
        btTransform t = mRigidBodyComp->mBody->getWorldTransform();
        t.setOrigin(btVector3(0, 3, 0));
        btQuaternion q;
        q.setEuler(0, 0, 0);
        t.setRotation(q);
        mRigidBodyComp->mBody->setWorldTransform(t);
        mRigidBodyComp->mBody->setLinearVelocity(btVector3(0, 0, 0));
        
        mLevelBuilder->refresh();
    }
}





