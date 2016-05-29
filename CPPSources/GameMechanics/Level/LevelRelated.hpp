#pragma once

#include "LevelRelated.hpp"
#include "GameObject.hpp"

namespace  LevelRelated {
    
    struct Action{
        enum Type{ NONE, YAW, JUMP, YAW_JUMP };
        
        Action(Type);
        Action(Type, q4d, float);
        Action(Type, q4d);
        Action(Type, float);

        
        Type    mType;
        q4d     mRotation;
        float   mMagnitude;
    };
    
    struct Candy : public IComponent{
        
        Candy(GameObject * const gameObject);
        virtual ~Candy();
        
        virtual void update() override;
        
        q4d initRotation{45, v3d(1, 0, 0)};
        
        v3d rotationAxis{0, 1, 0};
        
        float rotAngle{0.0f};
        
        void show(v3d position);
        void hide();
        
    private:
        
        void rotationAnim();
        
        float randomRotationAngle();
        
    };
    
    
}