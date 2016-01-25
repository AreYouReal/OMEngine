#pragma once

#include "GameObject.hpp"
#include "PhysicalWorld.hpp"
#include "Shortcuts.h"


class RigidBodyComponent : public IComponent{
public:
    RigidBodyComponent(GameObject * const gameObject, const float mass = 1.0f, PhysicContactCallback contactCC = nullptr, PhysicNearCallback nearCC = nullptr);
    
    virtual ~RigidBodyComponent();

    
    virtual bool init() override;
    virtual void update() override;
    virtual void draw() override;
    virtual void onDestroy() override;
    
    const m4d& transformMatrix() const { return mTransformMatrix; }
    
    btRigidBody             *mBody;
    
private:
    
    float                   mMass;
    PhysicContactCallback   mContactCC; // :)))))
    PhysicNearCallback      mNearCC;
    m4d                     mTransformMatrix;

};