#pragma once

#include "SRUtils.h"
#include "ObjMesh.h"
#include "Transform.hpp"

class GameObject{
    
public:
    GameObject();
    virtual ~GameObject();
    
    void addObjMesh(ObjMesh *objToAdd);
    
    void update();
    
private:
    
    Transform   transform;
    ObjMesh     *objMesh;
};