#pragma once

#include "IComponent.hpp"

#include "Scene.hpp"
#include "ArrowAction.hpp"

#include <queue>

class LevelBuilder : public IComponent{
public:

    // IComponent interface
    LevelBuilder(GameObject * const gameObject);
    virtual         ~LevelBuilder();
    // endof IComponent interface
    
    void InitWithMeshes(sp<ObjMesh> block, sp<ObjMesh> arrow);

    void buildLevel();
    
    ArrowAction * popAction();
    
private:
    
    std::queue<ArrowAction*> actions;
    
    
    std::vector<GameObject*> blockPool;
    std::vector<GameObject*> arrowPool;
    
    std::queue<GameObject*> activeblocks;
    std::queue<GameObject*> inactiveBlocks;
    
    std::queue<GameObject*> activeArrows;
    std::queue<GameObject*> inactiveArrows;
    
    unsigned int blockCount{50};
    
    GameObject *prevObj {nullptr};

    sp<ObjMesh> mBlock {nullptr};
    sp<ObjMesh> mArrow {nullptr};
    
    v3d mLastBlockPoss{0, 0, 0};
    v3d mLastDir{0, 0, 0};
    
    v3d  calculateNewPoss(v3d lastPos);
    void addNewBlock(v3d blockPos, float rotation);
    void addArrowToBlock(GameObject * const parent, float rotation);
    
    
    float getRotationAngle(v3d newPos, v3d lastDir);

};
