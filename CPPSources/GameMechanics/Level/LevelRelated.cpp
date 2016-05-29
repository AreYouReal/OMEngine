#include "LevelRelated.hpp"
#include "LevelBuilder.hpp"

using namespace LevelRelated;

Action::Action(Type type) : mType(type){};
Action::Action(Type type, q4d rotation, float magnitude)    : mType(type), mRotation(rotation), mMagnitude(magnitude){ }
Action::Action(Type type, q4d rotation)                     : Action(type, rotation, 0.0f){}
Action::Action(Type type, float magnitude)                  : Action(type, q4d(0, v3d(0, 1, 0)), magnitude){}

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