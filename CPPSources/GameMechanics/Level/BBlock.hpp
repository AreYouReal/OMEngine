#pragma once

#include "IComponent.hpp"
#include "m4d.h"
#include "LevelBuilder.hpp"

class BBlock : public IComponent {
public:
    
    enum BlockState{ FADE_IN, NORMAL, FADE_OUT };
    
    BBlock(GameObject * const, LevelBuilder * const);
    
    BBlock(GameObject * const);
    virtual ~BBlock();
    
    virtual void update() override;
    
    void setState(BlockState);

    void show(v3d);
    void hide();
    
    
private:
    LevelBuilder *mLB = nullptr;

    BlockState mState{NORMAL};

    float waitTime = 1.5f;
    
    float fadeInTime = .5f;
    float fadeOutTime = .5f;
    
    void fadeIn();
    void fadeOut();
    void normal();
};