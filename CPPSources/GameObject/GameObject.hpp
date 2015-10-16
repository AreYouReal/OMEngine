#pragma once

#include "SRUtils.h"
#include "ObjMesh.h"
#include "IRenderable.h"
#include "Camera.h"
#include "Transform.hpp"

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
    
private:
    
    std::vector<sp<GameObject>> mChildren;
    
    std::vector<sp<ObjMesh>>     mObjMeshes;
};