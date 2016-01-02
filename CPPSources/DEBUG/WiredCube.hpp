#pragma once

#include "MeshGLInfo.h"
#include "ObjMaterial.h"

class WiredCube{
public:
    WiredCube();
    void draw() const;
    
private:
  
    void buildVBO();
    void initMaterial();
    void setAttributes() const;
    
    static std::vector<v3d> vertices;
    
    sp<ObjMaterial> material;
    MeshGLInfo glInfo;
};