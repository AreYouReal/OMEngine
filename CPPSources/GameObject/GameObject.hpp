#pragma once

#include "SRUtils.h"
#include "ObjMesh.h"
#include "IRenderable.h"
#include "Camera.h"
#include "Transform.hpp"

// Bullet
#include "btRigidBody.h"

// Components
#include "IComponent.hpp"


class GameObject{
public:
    
    string name;
    
    GameObject(sp<Transform>, sp<ObjMesh>, string n = "noname");
    GameObject();
    ~GameObject();
    
    
    void addComponent(const ComponentEnum &name, const up<IComponent> comp);
    IComponent *getComponent(const ComponentEnum &name);
    
    
    void release();
    
    void addChild(sp<GameObject> child);
    void destroyChildren();
    
    Transform   mTransform{};
    
    btRigidBody *pBody = nullptr;          // Physics "body" of the mesh
    
    v3d getDimensions();
    
    v3d getPosition();
    
        std::vector<sp<GameObject>> mChildren;
    
private:

    std::map<ComponentEnum, up<IComponent>> mComponents;
};