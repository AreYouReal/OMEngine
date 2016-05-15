#pragma once

#include "IComponent.hpp"
#include "q4d.h"


class PlayerController;

struct ArrowAction : public IComponent{
    
    ArrowAction(GameObject * const gameObject);
    ArrowAction(GameObject * const gameObject, q4d rot);
    virtual ~ArrowAction();

    q4d rotation{0, v3d(0, 0, 1)};
    
    void apply(const PlayerController * ctr);
    void remove();
};