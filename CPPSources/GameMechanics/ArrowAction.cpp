#include "ArrowAction.hpp"

#include "Scene.hpp"

ArrowAction::ArrowAction(GameObject * const gameObject) : IComponent(gameObject){}

ArrowAction::ArrowAction(GameObject * const gameObject, q4d rot) : IComponent(gameObject){
    go->mTransform.mScale = v3d(0.5f, 0.5f, 0.5f);
    mActionRotation = rot;
    rotAngle = randomRotationAngle();
}

ArrowAction::~ArrowAction(){
    onDestroy();
    logMessage("ARROW COMPONENT DESTRUCTOR\n");
}

void ArrowAction::apply(const PlayerController *ctr){
        
}

void ArrowAction::show(v3d position, float rotation){
    rotAngle = randomRotationAngle();
    mActionRotation = q4d(rotation, v3d(0, 1, 0));
    go->mTransform.mPosition = position;
    (static_cast<MeshRendererComponent*>(go->getComponent(ComponentEnum::MESH_RENDERER)))->visible = true;
}

void ArrowAction::hide(){
    (static_cast<MeshRendererComponent*>(go->getComponent(ComponentEnum::MESH_RENDERER)))->visible = false;
}

void ArrowAction::update(){
    rotationAnim();
}


void ArrowAction::rotationAnim(){
    rotAngle += 250 * Time::deltaTime;
    if(rotAngle > 360) rotAngle = 0;
    q4d currentRotation = q4d(rotAngle, rotationAxis) * initRotation;
    go->mTransform.rotate(currentRotation);
}

float ArrowAction::randomRotationAngle(){
    return rand()%360;
}