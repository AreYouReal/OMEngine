#include "PlayerController.hpp"
#include "btSphereShape.h"
#include "Scene.hpp"
#include "LevelBuilder.hpp"
#include "BBlock.hpp"

std::vector<v3d> camFollowPositions{v3d(0, 10, 10), v3d(-10, 5, 5), v3d(5, 5, 10),v3d(5, 5, -10), v3d(-10, 5, -5)};

bool onPlayerPhysicalContact(btManifoldPoint &point, const btCollisionObjectWrapper *obj0, int part0, int index0, const btCollisionObjectWrapper *obj1, int part1, int index1){
    GameObject *go1 = ((GameObject*)((btCollisionObject*)obj1->getCollisionObject())->getUserPointer());
//    logMessage("Onbj1 : %s   <-> Obj2: %s \n", go0->name.c_str(), go1->name.c_str());
    
    BBlock* blockComp = static_cast<BBlock*>(go1->getComponent(ComponentEnum::BBLOCK));
    if(blockComp){
        blockComp->hide();
            return true;
    }
    
    LevelRelated::Candy* candy = static_cast<LevelRelated::Candy*>(go1->getComponent(ComponentEnum::CANDY));
    if(candy){
        candy->hide();
            return true;
    }
    
    return true;
}

PlayerController::PlayerController(GameObject * const gameObject) : IComponent(gameObject){
    mRigidBodyComp = static_cast<RigidBodyComponent*>(go->getComponent(ComponentEnum::RIGID_BODY));
    mRigidBodyComp->mBody->setAngularFactor(btVector3(0, 0, 0));
    mRigidBodyComp->mBody->setFriction(0.0);
    mRigidBodyComp->mBody->setRestitution(0.0);

    mAnimMeshComp = static_cast<AnimMeshComponent*>(go->getComponent(ComponentEnum::ANIM_MESH));
    
    startPose();
    
    btCollisionShape *shape = mRigidBodyComp->mBody->getCollisionShape();
    shape->setLocalScaling(btVector3(0.5f, 0.5f, 0.5f));
    
    
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

void PlayerController::setLevelBuilder(LevelBuilder *lb){
    mLevelBuilder = lb;
}

void PlayerController::activate(){
    mAnimMeshComp->setState(AnimMeshComponent::AnimationStates::JUMP, false);
    Camera::instance()->follow(go, camFollowPositions[1]);
    mRigidBodyComp->mBody->setGravity(btVector3(0, -9.8, 0));
    mActive = true;
    mRigidBodyComp->mBody->activate();
}

void PlayerController::onTouch(){
    if(mActive){
        mRigidBodyComp->mBody->activate();
        applyAction();
        refreshVelocity();
    }
}

void PlayerController::applyAction(){
    if(mRigidBodyComp){
        LevelRelated::Action act = mLevelBuilder->popAction();
        
        act.apply(mRigidBodyComp);

        switch (act.mType) {
            case LevelRelated::Action::Type::YAW_JUMP:
            {
                mAnimMeshComp->setState(AnimMeshComponent::AnimationStates::JUMP, false);
                currentFronVector = frontVector * act.mRotation.matrix();
                mJump = true;
            }
            break;
            case LevelRelated::Action::Type::YAW:
            {
                mAnimMeshComp->setState(AnimMeshComponent::AnimationStates::RUN, true);
                currentFronVector = frontVector * act.mRotation.matrix();
                mJump = false;
            break;
            }
            case LevelRelated::Action::Type::JUMP:
                mAnimMeshComp->setState(AnimMeshComponent::AnimationStates::JUMP, false);
                mJump = true;
            break;
            default:
            break;
        }
    }
}

void PlayerController::refreshVelocity(){
    if(mJump){
        mRigidBodyComp->mBody->setLinearVelocity(btVector3(currentFronVector.x * jumpSpeed, currentFronVector.y  + jumpSpeed/2.5, currentFronVector.z * jumpSpeed));
    }else{
        mRigidBodyComp->mBody->setLinearVelocity(btVector3(currentFronVector.x * playerSpeed, currentFronVector.y * playerSpeed, currentFronVector.z * playerSpeed));
    }

}

void PlayerController::refreshAnimMeshComp(){
    mAnimMeshComp = static_cast<AnimMeshComponent*>(go->getComponent(ComponentEnum::ANIM_MESH));
}

void PlayerController::update(){
    v3d pos = go->getPosition();
    if(pos.y < -3){
        startPose();
        mActive = false;
        mLevelBuilder->clearLevel();
        Scene::instance()->switchState(Scene::State::START_VIEW);
        Camera::instance()->follow(nullptr, v3d(0,0,0));
    }else{
        static float timer = 10.0f;
        if(timer < 0.0f){
            logMessage("Change camera follow position!\n");
            static int followIndex = 0;
            timer = 10.0f;
            ++followIndex;
            if(followIndex >= camFollowPositions.size()){
                followIndex = 1;
            }
//            Camera::instance()->follow(go, camFollowPositions[followIndex]);
            //            logMessage("Camera changing followIndex!\n");
            //            v3d::print(camFollowPositions[followIndex]);

        }
        
        timer -= Time::deltaTime;
    }
}

void PlayerController::startPose(){
    go->setPosition(v3d(0, 10, 0));
    btTransform t = mRigidBodyComp->mBody->getWorldTransform();
    btQuaternion q;
    q.setEuler(90, 0, 0);
    t.setRotation(q);
    mRigidBodyComp->mBody->setCenterOfMassTransform(t);
    mRigidBodyComp->mBody->setGravity(btVector3(0, 0, 0));
//    Camera::instance()->follow(go, camFollowPositions[0]);
    mRigidBodyComp->mBody->setLinearVelocity(btVector3(0, 0, 0));
    mAnimMeshComp->setState(AnimMeshComponent::AnimationStates::IDLE, true);
}
