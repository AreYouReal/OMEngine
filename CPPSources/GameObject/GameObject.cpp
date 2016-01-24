#include "GameObject.hpp"

// Logging
#include "OMUtils.h"



GameObject::GameObject(string name) : name(name){
    logMessage("GameObject constructor");
}

GameObject::GameObject(sp<Transform>, sp<ObjMesh>, string n) : name(n){ }

GameObject::~GameObject(){
    logMessage("GameObject destructor");
}

void GameObject::addChild(sp<GameObject> child){
    mChildren.push_back(std::move(child));
}

void GameObject::destroyChildren(){
    mChildren.clear();
}

void GameObject::addComponent(const ComponentEnum &name, up<IComponent> comp){
    mComponents.insert(std::pair<ComponentEnum, up<IComponent>>(name, std::move(comp)));
}

IComponent *GameObject::getComponent(const ComponentEnum &name){
    if(mComponents.find(name) != mComponents.end()) return mComponents[name].get();
    return nullptr;
}


v3d GameObject::getDimensions(){
//    v3d min, max;
//    for(auto const &mesh : mObjMeshes){
//        if(min.x > mesh->outlines.min.x) min.x = mesh->outlines.min.x;
//        if(min.y > mesh->outlines.min.y) min.y = mesh->outlines.min.y;
//        if(min.z > mesh->outlines.min.z) min.z = mesh->outlines.min.z;
//        if(max.x < mesh->outlines.max.x) max.x = mesh->outlines.max.x;
//        if(max.y < mesh->outlines.max.y) max.y = mesh->outlines.max.y;
//        if(max.z < mesh->outlines.max.z) max.z = mesh->outlines.max.z;
//    }
//    v3d::print(mObjMeshes[0]->outlines.max - mObjMeshes[0]->outlines.min);
//    
//    return (mObjMeshes[0]->outlines.max - mObjMeshes[0]->outlines.min);
    return v3d(1, 1, 1);
}


v3d GameObject::getPosition(){
    if(pBody){
        btVector3 origin = pBody->getWorldTransform().getOrigin();
        mTransform.mPosition = v3d(origin.x(), origin.y(), origin.z());
    }
    
    return mTransform.mPosition;
}