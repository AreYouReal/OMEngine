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
    
    
    void addComponent(const ComponentEnum &name, const up<IComponent> comp);
    IComponent *getComponent(const ComponentEnum &name);
    
    
    void release();
    
    void addChild(sp<GameObject> child);
    void destroyChildren();
    
    Transform   mTransform{};
    v3d getDimensions();
    v3d getPosition();
    
    m4d transformMatrix();
    
    std::vector<sp<GameObject>> mChildren;

private:

    std::map<ComponentEnum, up<IComponent>> mComponents;
};