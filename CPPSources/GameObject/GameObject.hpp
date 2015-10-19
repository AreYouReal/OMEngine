#pragma once

#include "SRUtils.h"
#include "ObjMesh.h"
#include "IRenderable.h"
#include "Camera.h"
#include "Transform.hpp"

// Bullet
#include "btRigidBody.h"


class GameObject : public IRenderable{
    
public:
    
    GameObject(sp<Transform>, sp<ObjMesh>);
    GameObject();
    virtual ~GameObject();
    
    void addObjMesh(sp<ObjMesh> objToAdd);
    
    void release();
    
    void addChild(sp<GameObject> child);
    void destroyChildren();
    
    virtual void draw() override;
    
    sp<Transform>   mTransform;
    
    btRigidBody *pBody = nullptr;          // Physics "body" of the mesh
    
    
    v3d getDimensions();
    
private:
    
    std::vector<sp<GameObject>> mChildren;
    
    std::vector<sp<ObjMesh>>    mObjMeshes;
};