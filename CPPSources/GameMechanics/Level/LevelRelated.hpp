#pragma once

#include "LevelRelated.hpp"
#include "GameObject.hpp"

namespace  LevelRelated {
    
    struct Action{
        enum Type{ YAW, JUMP };
        
        Type    mType;
        q4d     mRotation;
        float   mMagnitude;
    };
    
}