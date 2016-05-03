
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
    v3d position = go->getPosition();
    Camera::instance()->pushMMatrix( m4d::translate(position) * m4d::scale(go->getDimensions()) );
    wc.draw();
    Camera::instance()->popMMatrix();
}


void DebugDrawComponent::onDestroy(){ }