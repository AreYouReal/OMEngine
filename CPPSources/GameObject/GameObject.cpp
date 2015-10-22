#include "GameObject.hpp"

// Logging
#include "SRUtils.h"


GameObject::GameObject(){
    logMessage("GameObject constructor");
}

GameObject::GameObject(sp<Transform>, sp<ObjMesh>, str n) : name(n){ }

GameObject::~GameObject(){
    logMessage("GameObject destructor");
}

void GameObject::addChild(sp<GameObject> child){
    mChildren.push_back(std::move(child));
}

void GameObject::destroyChildren(){
    mChildren.clear();
}


void GameObject::draw(){
    // O_o
    m4d ATTRIBUTE_ALIGNED16(modelM);
    if(pBody){
        pBody->getWorldTransform().getOpenGLMatrix(modelM.pointer());
        modelM = m4d::transpose(modelM);
//        logMessage("%s %f, %f, %f\n", mObjMeshes[0]->getName().c_str(), pBody->getWorldTransform().m_origin[0], pBody->getWorldTransform().m_origin[1], pBody->getWorldTransform().m_origin[2]);
    }else {
        if(mTransform){ modelM = mTransform->transformMatrix(); }
        else{ logMessage("Something went wrong. There is no physical body or transform... "); }
    }

    
    Camera::instance()->pushMVMatrix(Camera::instance()->modelViewMatrix() * modelM);
    for (auto const& mesh : mObjMeshes) {
//        if(!Camera::instance()->sphereDistanceInFrustum(getPosition(), mesh->outlines.radius)){
//            logMessage("skip %s mesh\n", mesh->getName().c_str());
//            continue;
//        }
        if(mesh){
            if(mesh->renderObjectType() == RenderObjectType::SOLID){
                mesh->draw();
            }else{
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glCullFace(GL_FRONT);
                mesh->draw();
                glCullFace(GL_BACK);
                mesh->draw();
                glDisable(GL_BLEND);
            }
            
        }
    }

    for(auto const &child : mChildren){
        child->draw();
    }
    
    Camera::instance()->popMVMatrix();
}


void GameObject::addObjMesh(sp<ObjMesh> objToAdd){
    mObjMeshes.push_back(std::move(objToAdd));
}


v3d GameObject::getDimensions(){
    v3d min, max;
    for(auto const &mesh : mObjMeshes){
        if(min.x > mesh->outlines.min.x) min.x = mesh->outlines.min.x;
        if(min.y > mesh->outlines.min.y) min.y = mesh->outlines.min.y;
        if(min.z > mesh->outlines.min.z) min.z = mesh->outlines.min.z;
        if(max.x < mesh->outlines.max.x) max.x = mesh->outlines.max.x;
        if(max.y < mesh->outlines.max.y) max.y = mesh->outlines.max.y;
        if(max.z < mesh->outlines.max.z) max.z = mesh->outlines.max.z;
    }
    v3d::print(mObjMeshes[0]->outlines.max - mObjMeshes[0]->outlines.min);
    
    return (mObjMeshes[0]->outlines.max - mObjMeshes[0]->outlines.min);
}


v3d GameObject::getPosition(){
    if(pBody){
        btVector3 origin = pBody->getWorldTransform().getOrigin();
        return v3d(origin.x(), origin.y(), origin.z());
    }else{
        if(mTransform)
            return mTransform->mPosition;
    }
    return v3d();
}