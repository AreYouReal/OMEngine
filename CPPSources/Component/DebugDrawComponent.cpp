
#include "DebugDrawComponent.hpp"
#include "Camera.h"

DebugDrawComponent::DebugDrawComponent(GameObject * const gameObject, const v3d size) : IComponent(gameObject), mModelMatrix(m4d::scale(size)){}

DebugDrawComponent::~DebugDrawComponent(){
    onDestroy();
}

bool DebugDrawComponent::init(){
    return true;
}

void DebugDrawComponent::update(){
    Camera::instance()->pushMMatrix(mModelMatrix);
    wc.draw();
    Camera::instance()->popMMatrix();
}


void DebugDrawComponent::onDestroy(){ }