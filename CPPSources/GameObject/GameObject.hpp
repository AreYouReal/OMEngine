#pragma once

#include "SRUtils.h"
#include "ObjMesh.h"
#include "Transform.hpp"

class GameObject{
    
public:
    GameObject();
    virtual ~GameObject();
    
    void addObjMesh(sp<ObjMesh> objToAdd);
    
    void update();
    
private:
    
    sp<Transform>   transform;
    sp<ObjMesh> objMesh;
};