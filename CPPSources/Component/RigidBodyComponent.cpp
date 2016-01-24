#include "RigidBodyComponent.hpp"



RigidBodyComponent::RigidBodyComponent(GameObject * const gameObject) : IComponent(gameObject){
}

RigidBodyComponent::~RigidBodyComponent(){
    onDestroy();
}

void RigidBodyComponent::onDestroy(){

}

bool RigidBodyComponent::init(){
    
    if(!go){
        logMessage("Unable to init physical body for game object!");
        return false;
    }
    
    
    
    return true;
}

void RigidBodyComponent::update(){

}

//
//bool PhysicalWorld::addPBodyToGameObject(GameObject *go, PhysicalBodyShape shape, float mass, v3d dimension,PhysicContactCallback contactCallback, PhysicNearCallback nearCallback ){
//    if(!go) return false;
//    // Collision shape choosing goes here...
//    btCollisionShape *cShape = new btBoxShape(btVector3(dimension.x * 0.5f, dimension.y * 0.5f, dimension.z * 0.5f));
//    btTransform bttransform;
//    m4d transformM = m4d::transpose(go->mTransform.transformMatrix());
//    bttransform.setFromOpenGLMatrix(transformM.pointer());
//    btDefaultMotionState *defMState = new btDefaultMotionState(bttransform);
//    btVector3 localInertia(0.0f, 0.0f, 0.0f);
//    if(mass > 0.0f) cShape->calculateLocalInertia( mass, localInertia );
//    go->pBody = new btRigidBody(mass, defMState, cShape, localInertia);
//    go->pBody->setUserPointer(go);
//    physicsWorld->addRigidBody(go->pBody);
//    if(contactCallback) {
//        go->pBody->setCollisionFlags(go->pBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
//        gContactAddedCallback = contactCallback;
//    }
//    if(nearCallback) cDispatcher->setNearCallback(nearCallback);
//    return true;
//}