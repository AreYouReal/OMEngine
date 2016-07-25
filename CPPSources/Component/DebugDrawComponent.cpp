
#include "DebugDrawComponent.hpp"
#include "Camera.h"

DebugDrawComponent::DebugDrawComponent(GameObject * const gameObject) : IComponent(gameObject){
    init();
}

DebugDrawComponent::~DebugDrawComponent(){
    onDestroy();
}

void DebugDrawComponent::draw(){
    m4d transformM;
    
    RigidBodyComponent *rbc = static_cast<RigidBodyComponent*>(go->getComponent(ComponentEnum::RIGID_BODY));
    
    if(rbc){
        transformM = rbc->transformMatrix();
    }else{
        transformM = go->transformMatrix();
    }
    
    Camera::instance()->pushMMatrix( transformM );
    wc.draw();
    Camera::instance()->popMMatrix();
}


void DebugDrawComponent::onDestroy(){ }