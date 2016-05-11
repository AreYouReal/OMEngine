#include "ArrowAction.hpp"

#include "Scene.hpp"

ArrowAction::ArrowAction(){}

ArrowAction::ArrowAction(GameObject * go, q4d rot){
    arrowObj = go;
    rotation = rot;
}

void ArrowAction::apply(const PlayerController *ctr){
        
}

void ArrowAction::remove(){
    if(arrowObj != nullptr){
        Scene::instance()->removeObjectFromTheScene(arrowObj);
        arrowObj = nullptr;
    }
}
