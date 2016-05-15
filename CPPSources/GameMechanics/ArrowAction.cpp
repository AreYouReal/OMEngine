#include "ArrowAction.hpp"

#include "Scene.hpp"

ArrowAction::ArrowAction(GameObject * const gameObject) : IComponent(gameObject){}

ArrowAction::ArrowAction(GameObject * const gameObject, q4d rot) : IComponent(gameObject){
    rotation = rot;
}

ArrowAction::~ArrowAction(){
    onDestroy();
}

void ArrowAction::apply(const PlayerController *ctr){
        
}

void ArrowAction::hide(){
    (static_cast<MeshRendererComponent*>(go->getComponent(ComponentEnum::MESH_RENDERER)))->visible = false;
}