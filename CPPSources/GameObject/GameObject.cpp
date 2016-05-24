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
    v3d dimensions;
    MeshRendererComponent *mrc = static_cast<MeshRendererComponent *>(getComponent(ComponentEnum::MESH_RENDERER));
    if(mrc){
        dimensions = mrc->getDimensions();
        return v3d(dimensions.x * mTransform.mScale.x, dimensions.y * mTransform.mScale.y, dimensions.z * mTransform.mScale.z );
    }else{
        AnimMeshComponent *amc = static_cast<AnimMeshComponent *>(getComponent(ComponentEnum::ANIM_MESH));
        if(amc){
            dimensions = amc->dimension();
            return v3d(dimensions.x * mTransform.mScale.x, dimensions.y * mTransform.mScale.y, dimensions.z * mTransform.mScale.z );
        }
    }
    return dimensions;
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

v3d GameObject::getFront(){
    return mTransform.mFront;
}

void GameObject::setFront(v3d fronDir){
    mTransform.mFront = fronDir.normalize();
}

void GameObject::setPosition(v3d pos){
    RigidBodyComponent *rBody = static_cast<RigidBodyComponent*>( getComponent(ComponentEnum::RIGID_BODY) );
    if(rBody){
        btTransform t = rBody->mBody->getWorldTransform();
        t.setOrigin(btVector3(pos.x, pos.y, pos.z));
        rBody->mBody->setCenterOfMassTransform(t);
    }else{
        mTransform.mPosition = pos;
    }
}

m4d GameObject::transformMatrix(){
    RigidBodyComponent *rBody = static_cast<RigidBodyComponent*>( getComponent(ComponentEnum::RIGID_BODY) );
    if(rBody){ return rBody->transformMatrix() * mTransform.transformMatrix(); }
    
    return mTransform.transformMatrix();
}
