#include "ArrowAction.hpp"

#include "Scene.hpp"

ArrowAction::ArrowAction(GameObject * const gameObject) : IComponent(gameObject){}

ArrowAction::ArrowAction(GameObject * const gameObject, q4d rot) : IComponent(gameObject){
    go->mTransform.mScale = v3d(0.5f, 0.5f, 0.5f);
    mRotation = rot;
}

ArrowAction::~ArrowAction(){
    onDestroy();
    logMessage("ARROW COMPONENT DESTRUCTOR\n");
}

void ArrowAction::apply(const PlayerController *ctr){
        
}

void ArrowAction::show(v3d position, float rotation){
    mRotation = q4d(rotation, v3d(0, 1, 0));
    go->mTransform.mPosition = position;
    go->mTransform.rotate(mRotation);
    (static_cast<MeshRendererComponent*>(go->getComponent(ComponentEnum::MESH_RENDERER)))->visible = true;
}

void ArrowAction::hide(){
    (static_cast<MeshRendererComponent*>(go->getComponent(ComponentEnum::MESH_RENDERER)))->visible = false;
}