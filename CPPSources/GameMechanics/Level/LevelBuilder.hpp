#pragma once

#include "Scene.hpp"
#include "ArrowAction.hpp"

#include <queue>

class Levelbuilder{
public:
    unsigned int blockCount = 50;
    sp<ObjMesh> mesh;
    sp<ObjMesh> arrow;
    
    GameObject *prevObj;
    
    void buildLevel(std::queue<ArrowAction> &actions);
    
    
private:
    v3d mLastBlockPoss;
    v3d mLastDir;
    
    v3d  calculateNewPoss(v3d lastPos);
    void addNewBlock(v3d blockPos, const bool addArrow, ArrowAction *action);
    void addArrowToBlock(GameObject * const parent, ArrowAction *action);

};
