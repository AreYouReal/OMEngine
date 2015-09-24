#pragma once

#include "SRUtils.h"
#include "ObjMesh.h"

class GameObject{
    
public:
    GameObject();
    virtual ~GameObject();
    
    void addObjMesh(OBJMESH objToAdd);
    
    void draw();
    
private:
    OBJMESH objMesh;
};