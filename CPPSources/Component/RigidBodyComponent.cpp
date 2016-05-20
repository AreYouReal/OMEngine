#include "RigidBodyComponent.hpp"

RigidBodyComponent::RigidBodyComponent(GameObject * const gameObject, const float mass,PhysicContactCallback contactCC, PhysicNearCallback nearCC) : IComponent(gameObject),  mMass(mass), mContactCC(contactCC), mNearCC(nearCC){
    init();
}

RigidBodyComponent::~RigidBodyComponent(){
    onDestroy();
}

void RigidBodyComponent::onDestroy(){
    if(mBody){
        PhysicalWorld::instance()->removeBodyFromWorld(mBody);
        delete mBody->getCollisionShape();
        delete mBody->getMotionState();
        delete mBody;
    }
}

bool RigidBodyComponent::init(){
    
    if(!go){
        logMessage("Null object for RigidBody Component game object!");
        return false;
    }
    
    v3d size = go->getDimensions();
    m4d transformMatrix = m4d::transpose(go->mTransform.transformMatrix());
    
    
    btTransform transform;
    transform.setFromOpenGLMatrix(transformMatrix.pointer());
    
    
    btDefaultMotionState *defaultMotionState = new btDefaultMotionState(transform);
    btCollisionShape *collShape = new btBoxShape(btVector3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));
    
    btVector3 localInertia(0.0f, 0.0f, 0.0f);
    if(mMass > 0.0f){
        collShape->calculateLocalInertia(mMass, localInertia);
    }
    mBody = new btRigidBody(mMass, defaultMotionState, collShape, localInertia);
    mBody->setUserPointer(go);
    PhysicalWorld::instance()->addBodyToPhysicalWork(mBody);
    if(mContactCC){
        mBody->setCollisionFlags(mBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
        gContactAddedCallback = mContactCC;
    }
    if(mNearCC){ PhysicalWorld::instance()->addNearCallback(mNearCC); }
    
    
    m4d ATTRIBUTE_ALIGNED16(mTransformMatrix);
    return true;
}

void RigidBodyComponent::update(){
    if(mBody){
        mBody->getWorldTransform().getOpenGLMatrix((float *)mTransformMatrix.pointer());
        mTransformMatrix =  m4d::transpose(mTransformMatrix);
        //        logMessage("%s %f, %f, %f\n", mObjMeshes[0]->getName().c_str(), pBody->getWorldTransform().m_origin[0], pBody->getWorldTransform().m_origin[1], pBody->getWorldTransform().m_origin[2]);
    }
}

void RigidBodyComponent::draw(){}


void RigidBodyComponent::setContantCallback(PhysicContactCallback pcc){
    mContactCC = pcc;
    if(mContactCC){
        mBody->setCollisionFlags(mBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
        gContactAddedCallback = mContactCC;
    }
}

void RigidBodyComponent::setNearCallback(PhysicNearCallback pnc){
    mNearCC = pnc;
    if(mNearCC){ PhysicalWorld::instance()->addNearCallback(mNearCC); }
}