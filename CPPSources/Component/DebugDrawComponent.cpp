
#include "DebugDrawComponent.hpp"
#include "Camera.h"

DebugDrawComponent::DebugDrawComponent(GameObject * const gameObject) : IComponent(gameObject){
    init();
}

DebugDrawComponent::~DebugDrawComponent(){
    onDestroy();
}

bool DebugDrawComponent::init(){
    return true;
}

void DebugDrawComponent::update(){}

void DebugDrawComponent::draw(){
    Camera::instance()->pushMMatrix( m4d::translate(go->mTransform.mPosition) );
    wc.draw();
    Camera::instance()->popMMatrix();
}


void DebugDrawComponent::onDestroy(){ }