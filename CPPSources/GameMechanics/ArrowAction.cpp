#include "ArrowAction.hpp"

#include "Scene.hpp"

ArrowAction::ArrowAction(GameObject * const gameObject) : IComponent(gameObject){}

ArrowAction::ArrowAction(GameObject * const gameObject, q4d rot) : IComponent(gameObject){
    mRotation = rot;
}

ArrowAction::~ArrowAction(){
    onDestroy();
}

void ArrowAction::apply(const PlayerController *ctr){
        
}

void ArrowAction::show(v3d position, float rotation){
    mRotation = q4d(rotation, v3d(0, 0, 1));
    go->mTransform.rotate(mRotation);
    go->mTransform.mPosition = position;
    go->mTransform.refreshTransformMatrix();
    (static_cast<MeshRendererComponent*>(go->getComponent(ComponentEnum::MESH_RENDERER)))->visible = true;
}

void ArrowAction::hide(){
    (static_cast<MeshRendererComponent*>(go->getComponent(ComponentEnum::MESH_RENDERER)))->visible = false;
}