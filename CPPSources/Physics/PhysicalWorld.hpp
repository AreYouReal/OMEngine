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


/* Return true only if you change any variables of the contact point (such as the friction). */
using PhysicContactCallback = bool(*)(btManifoldPoint &point, const btCollisionObjectWrapper *obj0, int part0, int index0,
                               const btCollisionObjectWrapper *obj1, int part1, int index1);

using PhysicNearCallback = void(*)(btBroadphasePair &pair, btCollisionDispatcher &despatcher, const btDispatcherInfo &info );

class PhysicalWorld : public Singleton<PhysicalWorld>{

public:
    PhysicalWorld();
    PhysicalWorld(const PhysicalWorld& rhs) = delete;
    PhysicalWorld& operator=(const PhysicalWorld& rhs) = delete;
    ~PhysicalWorld();
    
    bool addPBodyToGameObject(sp<GameObject> go, PhysicalBodyShape shape, float mass, v3d dimension, PhysicContactCallback contactCallback = nullptr, PhysicNearCallback nearCallback = nullptr);
    
    void update(float deltaTime);
    
private:
    
    void init();
    void destroyTheWorld();
    
    
    sp<btSoftBodyRigidBodyCollisionConfiguration>   cConfig     = nullptr;
    sp<btCollisionDispatcher>                       cDispatcher = nullptr;
    sp<btBroadphaseInterface>                       cInterface  = nullptr;
    sp<btConstraintSolver>                          cSolver     = nullptr;
    sp<btSoftRigidDynamicsWorld>                    physicsWorld= nullptr;
};