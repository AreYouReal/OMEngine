#pragma once

#include "GameObject.hpp"

class PlayerController;

struct ArrowAction{
    
    ArrowAction();
    ArrowAction(GameObject * go, q4d rot);
    
    GameObject *arrowObj = nullptr;
    q4d rotation{0, v3d(0, 0, 1)};
    
    void apply(const PlayerController * ctr);
    void remove();
};