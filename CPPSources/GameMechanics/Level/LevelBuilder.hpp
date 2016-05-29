#pragma once

#include "IComponent.hpp"

#include "Scene.hpp"
#include "ArrowAction.hpp"

#include <queue>

#include "LevelRelated.cpp"

class LevelBuilder : public IComponent{
public:

    // IComponent interface
    LevelBuilder(GameObject * const gameObject);
    virtual         ~LevelBuilder();
    
    virtual void update() override;
    
    // endof IComponent interface
    
    void InitWithMeshes(sp<ObjMesh> block, sp<ObjMesh> arrow);

    void buildLevel();
    
    void onHideBlock(GameObject *blockOBj);
    
    LevelRelated::Action popAction();
    
    void refresh();
    
    void clearLevel();
    
private:
    
    std::queue<LevelRelated::Action> actions;
    
    std::vector<GameObject*> activeblocks;
    std::vector<GameObject*> inactiveBlocks;
    
    std::vector<GameObject*> activeArrows;
    std::vector<GameObject*> inactiveArrows;
    
    unsigned int blockCount{15};
    
    GameObject *prevObj {nullptr};

    sp<ObjMesh> mBlock {nullptr};
    sp<ObjMesh> mArrow {nullptr};
    
    v3d mLastBlockPoss{0, 0, 0};
    v3d mLastDir{0, 0, 0};
    
    v3d  calculateNewPoss(v3d lastPos);
    void addNewBlock(v3d blockPos, float rotation);
    void addArrowToBlock(float rotation);
    
    
    float getRotationAngle(v3d newPos, v3d lastDir);

    void addBlockComponent(GameObject*);
    void activateBlock(GameObject*);
    

    
};
