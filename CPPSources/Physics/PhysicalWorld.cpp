
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

void PhysicalWorld::update(){
    physicsWorld->stepSimulation( 1.0f / 60.f );
}

bool PhysicalWorld::addPBodyToGameObject(sp<GameObject> go, PhysicalBodyShape shape, float mass, v3d dimension){
    if(!go) return false;
    v3d goDimensions = go->getDimensions();
    
    // Collision shape choosing goes here...
    btCollisionShape *cShape = new btBoxShape(btVector3(goDimensions.x * 0.5f, goDimensions.y * 0.5f, goDimensions.z * 0.5f));
    btTransform bttransform;
    m4d transformM = m4d::transpose(go->mTransform->transformMatrix());
    bttransform.setFromOpenGLMatrix(transformM.pointer());
    btDefaultMotionState *defMState = new btDefaultMotionState(bttransform);
    btVector3 localInertia(0.0f, 0.0f, 0.0f);
    if(mass > 0.0f) cShape->calculateLocalInertia( mass, localInertia );
    go->pBody = new btRigidBody(mass, defMState, cShape, localInertia);
    go->pBody->setUserPointer(go.get());
    physicsWorld->addRigidBody(go->pBody);
    return true;
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