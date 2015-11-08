#pragma once

#include "SRUtils.h"
#include "MeshGLInfo.h"
#include "ObjMaterial.h"

class WiredCube{
public:
    WiredCube();
    void draw();
    
private:
  
    void buildVBO();
    void initMaterial();
    void setAttributes();
    
    static std::vector<v3d> vertices;
    
    sp<ObjMaterial> material;
    MeshGLInfo glInfo;
};