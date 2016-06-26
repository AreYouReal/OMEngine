#include "GameObject.hpp"

// Logging
#include "OMUtils.h"



GameObject::GameObject(string name) : name(name){
    logMessage("GameObject constructor\n");
}

GameObject::~GameObject(){
    logMessage("GameObject destructor\n");
}


void GameObject::update(){
    if(!mActive) return;
    
    for(auto const& comp : mComponents){
        comp.second->update();
    }
    
    for(auto const& child : mChildren){
        child->update();
    }
}

void GameObject::draw(){
    if(!mActive) return;
    
    for(auto const& comp : mComponents){
        comp.second->draw();
    }
    
    for(auto const& child : mChildren){
        child->draw();
    }
}

void GameObject::addChild(up<GameObject> child){
    child->parent = this;
    mChildren.push_back(std::move(child));
}

void GameObject::destroyChildren(){
    mChildren.clear();
}

IComponent* GameObject::addComponent(up<IComponent> comp){
    IComponent *returnThis = comp.get();
    mComponents.insert(std::pair<ComponentEnum, up<IComponent>>(comp->mComponentType, std::move(comp)));
    return returnThis;
}

void GameObject::removeComponent(ComponentEnum comp){
    if(mComponents.find(comp) != mComponents.end()){
        mComponents.erase(comp);
    }
}

IComponent *GameObject::getComponent(const ComponentEnum &name){
    if(mComponents.find(name) != mComponents.end()) return mComponents[name].get();
    return nullptr;
}


v3d GameObject::getDimensions(){
    v3d dimensions(0.1f, 0.1f, 0.1f);
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
    m4d m = mTransform.transformMatrix();
    if(parent != nullptr)
        m =  parent->transformMatrix() * m;
    
    
    RigidBodyComponent *rBody = static_cast<RigidBodyComponent*>( getComponent(ComponentEnum::RIGID_BODY) );
    if(rBody){ return  rBody->transformMatrix() * m; }
    
    return m;
}
