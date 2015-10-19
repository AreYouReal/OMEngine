#pragma once

#include "Singleton.hpp"

// Bullet headers
#include "btSoftBodyRigidBodyCollisionConfiguration.h"

#include "btDbvtBroadphase.h"
#include "btCollisionDispatcher.h"
#include "btBroadphaseInterface.h"
#include "btConstraintSolver.h"
#include "btSequentialImpulseConstraintSolver.h"
#include "btSoftRigidDynamicsWorld.h"

#include "btBoxShape.h"

#include "btDefaultMotionState.h"



class PhysicalWorld : public Singleton<PhysicalWorld>{
public:
    PhysicalWorld();
    PhysicalWorld(const PhysicalWorld& rhs) = delete;
    PhysicalWorld& operator=(const PhysicalWorld& rhs) = delete;
    ~PhysicalWorld();
    
    
private:
    
    void init();
    void destroyTheWorld();
    
    
    sp<btSoftBodyRigidBodyCollisionConfiguration>   cConfig     = nullptr;
    sp<btCollisionDispatcher>                       cDispatcher = nullptr;
    sp<btBroadphaseInterface>                       cInterface  = nullptr;
    sp<btConstraintSolver>                          cSolver     = nullptr;
    sp<btSoftRigidDynamicsWorld>                    physicsWorld= nullptr;
};