
#include "PhysicalWorld.hpp"

#include "btManifoldPoint.h"

#include "OMUtils.h"
#include "Game.h"

#include "btBulletWorldImporter.h"

PhysicalWorld::PhysicalWorld(){
    init();
    logMessage("PhysicalWorld constructor!\n");
}

PhysicalWorld::~PhysicalWorld(){
    destroyTheWorld();
    logMessage("PhysicalWorld destructor!\n");
}

void PhysicalWorld::update(float deltaTime){
    physicsWorld->stepSimulation( deltaTime );
}

bool PhysicalWorld::addPBodyToGameObject(GameObject *go, PhysicalBodyShape shape, float mass, v3d dimension,PhysicContactCallback contactCallback, PhysicNearCallback nearCallback ){
    if(!go) return false;
    // Collision shape choosing goes here...
    btCollisionShape *cShape = new btBoxShape(btVector3(dimension.x * 0.5f, dimension.y * 0.5f, dimension.z * 0.5f));
    btTransform bttransform;
    m4d transformM = m4d::transpose(go->mTransform.transformMatrix());
    bttransform.setFromOpenGLMatrix(transformM.pointer());
    btDefaultMotionState *defMState = new btDefaultMotionState(bttransform);
    btVector3 localInertia(0.0f, 0.0f, 0.0f);
    if(mass > 0.0f) cShape->calculateLocalInertia( mass, localInertia );
    go->pBody = new btRigidBody(mass, defMState, cShape, localInertia);
    go->pBody->setUserPointer(go);
    physicsWorld->addRigidBody(go->pBody);
    if(contactCallback) {
        go->pBody->setCollisionFlags(go->pBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
        gContactAddedCallback = contactCallback;
    }
    if(nearCallback) cDispatcher->setNearCallback(nearCallback);
    return true;
}

bool PhysicalWorld::loadPhysicsWorldFromFile(string filename, std::vector<GameObject*> objects){
    up<FileContent> fileContent = readBytesFromFile(Game::getAppContext(), filename.c_str());
    if(!fileContent) return false;
    
    btBulletWorldImporter *bulletImporter = new btBulletWorldImporter(physicsWorld.get());
    bulletImporter->loadFileFromMemory(const_cast<char*>(fileContent->content), fileContent->size);
    
    for(unsigned short i = 0; i < bulletImporter->getNumRigidBodies(); ++i){
        btCollisionObject *co = bulletImporter->getRigidBodyByIndex(i);
        string name = bulletImporter->getNameForPointer(co);
        
        for(const auto go : objects){
            if(!go->name.compare(name)){
                go->pBody = (btRigidBody *)co;
                go->pBody->setUserPointer(go);
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

