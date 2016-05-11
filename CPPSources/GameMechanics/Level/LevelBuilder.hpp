#pragma once

#include "Scene.hpp"

#include <queue>

class Levelbuilder{
public:
    unsigned int blockCount = 50;
    sp<ObjMesh> mesh;
    sp<ObjMesh> arrow;
    
    GameObject *prevObj;
    
    void buildLevel(std::queue<float> &actions);
    
    
private:
    v3d mLastBlockPoss;
    v3d mLastDir;
    
    v3d  calculateNewPoss(v3d lastPos);
    void addNewBlock(v3d blockPos, const bool addArrow,const float rotation);
    void addArrowToBlock(GameObject * const parent, const float rotation);

};
