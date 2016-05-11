#pragma once

#include "GameObject.hpp"
#include "PlayerController.hpp"

struct Action{
    sp<GameObject> arrowObj = nullptr;
    q4d rotation{0, v3d(0, 0, 1)};
    
    void apply(const PlayerController * ctr);
    void remove();
};