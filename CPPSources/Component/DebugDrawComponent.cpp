
#include "DebugDrawComponent.hpp"
#include "Camera.h"

DebugDrawComponent::DebugDrawComponent(GameObject * const gameObject) : IComponent(gameObject){
    init();
}

DebugDrawComponent::~DebugDrawComponent(){
    onDestroy();
}

void DebugDrawComponent::draw(){
    v3d position = go->getPosition();
    v3d dimesions = go->getDimensions();
    Camera::instance()->pushMMatrix( go->transformMatrix() );
    wc.draw();
    Camera::instance()->popMMatrix();
}


void DebugDrawComponent::onDestroy(){ }