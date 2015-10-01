#include "ASceneNode.hpp"

#include "Camera.h"

ASceneNode::ASceneNode(Transform *tr, ObjMesh *m) : transform(tr), mesh(m){ }

ASceneNode::~ASceneNode(){
    destroyChildren();
}

void ASceneNode::release(){
    delete this;
}

void ASceneNode::addChild(ASceneNode *child){
    mChildren.push_back(child);
}

void ASceneNode::destroyChildren(){
    for(auto child : mChildren){
        child->release();
    }
    mChildren.clear();
}

#pragma mark virtual
void ASceneNode::update(){
    m4d modelM;
    if(transform){
        modelM = transform->transformMatrix();
    }
    Camera::instance()->pushMVMatrix(Camera::instance()->modelViewMatrix() * modelM);
    if(mesh){
        mesh->draw();
    }
    for(auto child : mChildren){
        child->update();
    }
    Camera::instance()->popMVMatrix();
}

