#include "GameObject.hpp"


GameObject::GameObject(){
    logMessage("GameObject constructor");
}

GameObject::GameObject(sp<Transform>, sp<ObjMesh>) : GameObject(){
    
}

GameObject::~GameObject(){
    logMessage("GameObject destructor");
}

void GameObject::addChild(sp<GameObject> child){
    mChildren.push_back(std::move(child));
}

void GameObject::destroyChildren(){
    mChildren.clear();
}


void GameObject::draw(){
    m4d modelM;
    if(mTransform){ modelM = mTransform->transformMatrix(); }
    
    Camera::instance()->pushMVMatrix(Camera::instance()->modelViewMatrix() * modelM);
    for (auto const& mesh : mObjMeshes) {
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
    }

    for(auto const &child : mChildren){
        child->draw();
    }
    
    Camera::instance()->popMVMatrix();
}


void GameObject::addObjMesh(sp<ObjMesh> objToAdd){
    mObjMeshes.push_back(std::move(objToAdd));
}

