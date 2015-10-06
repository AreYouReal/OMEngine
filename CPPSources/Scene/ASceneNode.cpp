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
    if(transform){ modelM = transform->transformMatrix(); }
    
    Camera::instance()->pushMVMatrix(Camera::instance()->modelViewMatrix() * modelM);

    if(mesh){
        if(mesh->renderObjectType() == RenderObjectType::SOLID){
            mesh->draw();
        }else{
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glCullFace(GL_FRONT);
            mesh->draw();
            glCullFace(GL_BACK);
            mesh->draw();
            glDisable(GL_BLEND);
        }

    }
    for(auto child : mChildren){
        child->update();
    }
    
    
    Camera::instance()->popMVMatrix();
}




