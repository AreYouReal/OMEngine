
#include "PhysicalWorld.hpp"

#include "SRUtils.h"

PhysicalWorld::PhysicalWorld(){
    init();
    logMessage("PhysicalWorld constructor!\n");
}

PhysicalWorld::~PhysicalWorld(){
    destroyTheWorld();
    logMessage("PhysicalWorld destructor!\n");
}

#pragma mark Helpers
void PhysicalWorld::init(){
    cConfig      = std::make_shared<btSoftBodyRigidBodyCollisionConfiguration>();
    cDispatcher  = std::make_shared<btCollisionDispatcher>(cConfig.get());
    cInterface   = std::make_shared<btDbvtBroadphase>();
    cSolver      = std::make_shared<btSequentialImpulseConstraintSolver>();
    
    physicsWorld = std::make_shared<btSoftRigidDynamicsWorld>(cDispatcher.get(),
                                                              cInterface.get(),
                                                              cSolver.get(),
                                                              cConfig.get());
    physicsWorld->setGravity(btVector3(0, 0, -9.8f));
}

void PhysicalWorld::destroyTheWorld(){
    while(physicsWorld->getNumCollisionObjects()){
        btCollisionObject *btCObject = physicsWorld->getCollisionObjectArray()[0];
        btRigidBody *rBody = btRigidBody::upcast(btCObject);
        if(rBody){
            delete rBody->getCollisionShape();
            delete rBody->getMotionState();
            physicsWorld->removeRigidBody(rBody);
            physicsWorld->removeCollisionObject(btCObject);
            delete rBody;
        }
    }
}