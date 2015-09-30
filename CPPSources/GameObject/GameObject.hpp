#pragma once

#include "SRUtils.h"
#include "ObjMesh.h"

class GameObject{
    
public:
    GameObject();
    virtual ~GameObject();
    
    void addObjMesh(ObjMesh *objToAdd);
    
    void draw();
    
private:
    ObjMesh *objMesh;
};