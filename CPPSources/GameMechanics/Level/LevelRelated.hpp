#pragma once

#include "GameObject.hpp"

class LevelBuilder;


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
        
        btQuaternion rotationCorrection{btVector3(0, 1, 0), 0.5f * PI};

        void apply(RigidBodyComponent * const rBody);
    
    private:
        
        void yawAction(RigidBodyComponent * const rBody);
        void yawAndJumpAction(RigidBodyComponent * const rBody);
        void jumpAction(RigidBodyComponent * const rBody);
    };
    
    struct Candy : public IComponent{
        
        Candy(GameObject * const gameObject, LevelBuilder * const);
        virtual ~Candy();
        
        virtual bool init()     override;
        virtual void update()   override;
        
        
        q4d initRotation{45, v3d(1, 0, 0)};
        
        v3d rotationAxis{0, 1, 0};
        
        float rotAngle{0.0f};
        
        void show(v3d position);
        void hide();
        
    private:
        LevelBuilder *mLevelBuilder;
        
        RigidBodyComponent *mRigiBodyComp;
        
        void rotationAnim();
        
        float randomRotationAngle();
        
    };
    
    
}