#include "ASceneNode.hpp"

#include "Camera.h"

ASceneNode::ASceneNode(sp<Transform> tr, sp<ObjMesh> m) : transform(tr), mesh(m){ }

ASceneNode::~ASceneNode(){
    destroyChildren();
}

void ASceneNode::release(){
    delete this;
}

void ASceneNode::addChild(up<ASceneNode> child){
    mChildren.push_back(std::move(child));
}

void ASceneNode::destroyChildren(){
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
    for(auto const &child : mChildren){
        child->update();
    }
    
    
    Camera::instance()->popMVMatrix();
}




