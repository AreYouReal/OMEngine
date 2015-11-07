#pragma once

#include "SRUtils.h"
#include "MeshGLInfo.h"

class WiredCube{
public:
    void draw();

private:
    
    static std::vector<v3d>             vertices;
    static std::vector<unsigned short>  indices;
    
    MeshGLInfo mInfo;
};