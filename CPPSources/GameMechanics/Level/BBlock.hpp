#pragma once

#include "IComponent.hpp"
#include "m4d.h"

class BBlock : public IComponent {
public:
    BBlock(GameObject * const go);
    virtual ~BBlock();
    
    void reinit(v3d newPos);
    void show();
    void hide();    
};