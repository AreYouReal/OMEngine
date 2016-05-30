#include "LevelRelated.hpp"
#include "LevelBuilder.hpp"

using namespace LevelRelated;

Action::Action(Type type) : mType(type){};
Action::Action(Type type, q4d rotation, float magnitude)    : mType(type), mRotation(rotation), mMagnitude(magnitude){ }
Action::Action(Type type, q4d rotation)                     : Action(type, rotation, 0.0f){}
Action::Action(Type type, float magnitude)                  : Action(type, q4d(0, v3d(0, 1, 0)), magnitude){}


void Action::apply(RigidBodyComponent *rBody){
    if(rBody == nullptr) return;
    
    switch (mType) {
        case YAW:
            yawAction(rBody);
            break;
        case JUMP:
            jumpAction(rBody);
            break;
        case YAW_JUMP:
            yawAndJumpAction(rBody);
            break;
        default:
            break;
    }
}

void Action::yawAction(RigidBodyComponent * const rBody){
    btTransform t = rBody->mBody->getWorldTransform();
    btQuaternion currQ = t.getRotation();
    btQuaternion q(mRotation.x, mRotation.y, mRotation.z, mRotation.w);
    currQ = q * rotationCorrection;
    t.setRotation(currQ);
    rBody->mBody->setWorldTransform(t);
}

void Action::jumpAction(RigidBodyComponent * const rBody){
    rBody->mBody->applyCentralForce(btVector3(0, mMagnitude, 0));
}

void Action::yawAndJumpAction(RigidBodyComponent * const rBody){
    btTransform t = rBody->mBody->getWorldTransform();
    btQuaternion currQ = t.getRotation();
    rBody->mBody->applyCentralForce(btVector3(0, mMagnitude, 0));
    btQuaternion q(mRotation.x, mRotation.y, mRotation.z, mRotation.w);
    currQ = q * rotationCorrection;
    t.setRotation(currQ);
    rBody->mBody->setWorldTransform(t);
}


Candy::Candy(GameObject * const gameObject, LevelBuilder * const builder) : IComponent(gameObject){
    rotAngle = randomRotationAngle();
    mLevelBuilder = builder;
}

Candy::~Candy(){
    onDestroy();
    logMessage("ARROW COMPONENT DESTRUCTOR\n");
}

void Candy::show(v3d position){
    rotAngle = randomRotationAngle();
    go->setPosition(position);
    (static_cast<MeshRendererComponent*>(go->getComponent(ComponentEnum::MESH_RENDERER)))->visible = true;
}

void Candy::hide(){
    (static_cast<MeshRendererComponent*>(go->getComponent(ComponentEnum::MESH_RENDERER)))->visible = false;
    mLevelBuilder->onHideCandy(go);
}

void Candy::update(){
    rotationAnim();
}


void Candy::rotationAnim(){
//    rotAngle += 250 * Time::deltaTime;
//    if(rotAngle > 360) rotAngle = 0;
//    q4d currentRotation = q4d(rotAngle, rotationAxis);
//    go->mTransform.rotate(currentRotation);
}

float Candy::randomRotationAngle(){
    return rand()%360;
}