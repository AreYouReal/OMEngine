
#include "PhysicalWorld.hpp"

#include "btManifoldPoint.h"

#include "OMUtils.h"

#include "btBulletWorldImporter.h"

PhysicalWorld::PhysicalWorld(){
    init();
}

PhysicalWorld::~PhysicalWorld(){
    destroyTheWorld();
}

void PhysicalWorld::update(float deltaTime){
    physicsWorld->updateAabbs();
    physicsWorld->stepSimulation( deltaTime );
}

bool PhysicalWorld::addBodyToPhysicalWork(btRigidBody *body){
    physicsWorld->addRigidBody(body);
    return true;
}

void PhysicalWorld::addNearCallback(PhysicNearCallback nearCC){
    cDispatcher->setNearCallback(nearCC);
}


bool PhysicalWorld::loadPhysicsWorldFromFile(string filename, std::vector<GameObject*> objects){
    up<FileContent> fileContent = readBytesFromFile(filename.c_str());
    if(!fileContent) return false;
    
    btBulletWorldImporter *bulletImporter = new btBulletWorldImporter(physicsWorld.get());
    bulletImporter->loadFileFromMemory((char*)fileContent->content, fileContent->size);
    
    for(unsigned short i = 0; i < bulletImporter->getNumRigidBodies(); ++i){
        btCollisionObject *co = bulletImporter->getRigidBodyByIndex(i);
        string name = bulletImporter->getNameForPointer(co);
        
        for(const auto go : objects){
            if(!go->name.compare(name)){
//                go->pBody = (btRigidBody *)co;
//                go->pBody->setUserPointer(go);
            }
        }
        
        logMessage("Collision shape name loaded from file: %s\n" , name.c_str());
    }
    delete bulletImporter;

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

void PhysicalWorld::removeBodyFromWorld(btRigidBody *body){
    if(body){
        physicsWorld->removeRigidBody(body);
        physicsWorld->removeCollisionObject(body);
    }
}

