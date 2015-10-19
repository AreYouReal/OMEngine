#pragma once

#include "Singleton.hpp"
#include "GameObject.hpp"

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

enum class PhysicalBodyShape{ BOX, SPHERE };

class PhysicalWorld : public Singleton<PhysicalWorld>{

public:
    PhysicalWorld();
    PhysicalWorld(const PhysicalWorld& rhs) = delete;
    PhysicalWorld& operator=(const PhysicalWorld& rhs) = delete;
    ~PhysicalWorld();
    
    bool addPBodyToGameObject(sp<GameObject> go, PhysicalBodyShape shape, float mass, v3d dimension);
    
    void update();
    
private:
    
    void init();
    void destroyTheWorld();
    
    
    sp<btSoftBodyRigidBodyCollisionConfiguration>   cConfig     = nullptr;
    sp<btCollisionDispatcher>                       cDispatcher = nullptr;
    sp<btBroadphaseInterface>                       cInterface  = nullptr;
    sp<btConstraintSolver>                          cSolver     = nullptr;
    sp<btSoftRigidDynamicsWorld>                    physicsWorld= nullptr;
};