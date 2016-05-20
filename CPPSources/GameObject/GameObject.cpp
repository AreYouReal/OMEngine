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
    child->parent = this;
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
    MeshRendererComponent *mrc = static_cast<MeshRendererComponent *>(getComponent(ComponentEnum::MESH_RENDERER));
    if(mrc){
        return mrc->getDimensions();
    }else{
        AnimMeshComponent *amc = static_cast<AnimMeshComponent *>(getComponent(ComponentEnum::ANIM_MESH));
        if(amc){
//            return amc->md5->dimension;
        }
    }
    return v3d(1, 1, 1);
}


v3d GameObject::getPosition(){
    RigidBodyComponent *rBody = static_cast<RigidBodyComponent*>( getComponent(ComponentEnum::RIGID_BODY) );
    if(rBody){
        btVector3 origin = rBody->mBody->getWorldTransform().getOrigin();
        mTransform.mPosition = v3d(origin.x(), origin.y(), origin.z());
    }
    v3d parentPos(0, 0, 0);
    if(parent != nullptr)
        parentPos = parent->getPosition();
        
    return (mTransform.mPosition + parentPos);
}

m4d GameObject::transformMatrix(){
    RigidBodyComponent *rBody = static_cast<RigidBodyComponent*>( getComponent(ComponentEnum::RIGID_BODY) );
    if(rBody){ return /*mTransform.transformMatrix() **/ rBody->transformMatrix(); }
    
    return mTransform.transformMatrix();
}
