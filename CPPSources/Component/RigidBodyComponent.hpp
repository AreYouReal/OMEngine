#pragma once

#include "GameObject.hpp"
#include "PhysicalWorld.hpp"
#include "Shortcuts.h"


class RigidBodyComponent : public IComponent{
public:
    RigidBodyComponent(GameObject * const gameObject, const float mass = 1.0f, PhysicContactCallback contactCC = nullptr, PhysicNearCallback nearCC = nullptr);
    
    virtual ~RigidBodyComponent();

    

    
    virtual void update() override;
    virtual void draw() override;
    virtual void onDestroy() override;
    
    bool initRigidBody();
    
    const m4d& transformMatrix() const { return mTransformMatrix; }
    
    btRigidBody             *mBody;
    
    void setContantCallback(PhysicContactCallback pcc);
    void setNearCallback(PhysicNearCallback pnc);
    
    void updateTransformMatrix();

private:
    
    float                   mMass;
    PhysicContactCallback   mContactCC; // :)))))
    PhysicNearCallback      mNearCC;
    m4d ATTRIBUTE_ALIGNED16(mTransformMatrix);
};