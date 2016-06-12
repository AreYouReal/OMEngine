#pragma once

#include "IComponent.hpp"

#include "Scene.hpp"

#include <queue>


#include "LevelRelated.hpp"

using LAction = LevelRelated::Action;

class LevelBuilder : public IComponent{
public:

    // IComponent interface
    LevelBuilder(GameObject * const gameObject);
    virtual         ~LevelBuilder();
    
    virtual void update() override;
    
    // endof IComponent interface
    
    void InitWithMeshes(sp<ObjMesh> block, std::vector<sp<ObjMesh>> candies);

    void createFirstBlock();
    
    void buildLevel();
    
    void onHideBlock(GameObject *blockOBj);
    
    void onHideCandy(GameObject *candyOBj);
    
    LAction popAction();
    
    void refresh();
    
    void clearLevel();
    
private:
    
    std::queue<LevelRelated::Action> actions;
    
    std::vector<GameObject*> activeblocks;
    std::vector<GameObject*> inactiveBlocks;
    
    std::vector<GameObject*> activeCandies;
    std::vector<GameObject*> inactiveCandies;
    
    unsigned int blockCount{15};
    
    GameObject *prevObj {nullptr};

    sp<ObjMesh> mBlock {nullptr};
    std::vector<sp<ObjMesh>> mCandies {nullptr};
    
    v3d mLastBlockPoss{0, 0, 0};
    v3d mLastDir{0, 0, 0};
    
    
    float mCandyChance = 50.0f;
    
    v3d  calculateNewPoss(v3d lastPos);
    void addNewBlock(v3d blockPos, LAction action);
    void addCandyToBlock();
    
    
    LevelRelated::Action getAction(v3d newPos, v3d lastDir);

    void addBlockComponent(GameObject*);
    void activateBlock(GameObject*);
    

    
};
