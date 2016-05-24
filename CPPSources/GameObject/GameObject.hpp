#pragma once

#include "OMUtils.h"
#include "ObjMesh.h"
#include "Camera.h"
#include "Transform.hpp"

// Bullet
#include "btRigidBody.h"

// Components
#include "IComponent.hpp"
#include "RigidBodyComponent.hpp"
#include "MeshRendererComponent.hpp"
#include "AnimMeshComponent.hpp"


class GameObject{
public:
    
    string name;
    GameObject(string name = "noname");
    GameObject(sp<Transform>, sp<ObjMesh>, string n = "noname");
    ~GameObject();
    
    
    void addComponent(const ComponentEnum&, const up<IComponent>);
    IComponent *getComponent(const ComponentEnum&);
    
    
    GameObject *parent = nullptr;
    
    void release();
    
    void addChild(sp<GameObject>);
    void destroyChildren();
    
    Transform   mTransform{};
    v3d getDimensions();
    v3d getPosition();
    v3d getFront();
    
    void setPosition(v3d);
    void setFront(v3d);
    
    m4d transformMatrix();
    
    std::vector<sp<GameObject>> mChildren;

    std::map<ComponentEnum, up<IComponent>> mComponents;
};