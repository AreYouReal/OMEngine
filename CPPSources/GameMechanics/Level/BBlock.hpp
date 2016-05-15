#pragma once

#include "IComponent.hpp"
#include "m4d.h"
#include "LevelBuilder.hpp"

class BBlock : public IComponent {
public:
    
    BBlock(GameObject * const gameObject, LevelBuilder * const lb);
    
    BBlock(GameObject * const go);
    virtual ~BBlock();
    
    void reinit(v3d newPos);
    void show();
    void hide();
    
    
private:
    LevelBuilder *mLB = nullptr;
};