#include "GameObject.hpp"

// Logging
#include "SRUtils.h"


GameObject::GameObject(){
    logMessage("GameObject constructor");
}

GameObject::GameObject(sp<Transform>, sp<ObjMesh>) : GameObject(){
    
}

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
    m4d modelM;
    if(pBody){
        pBody->getWorldTransform().getOpenGLMatrix(modelM.pointer());
        modelM = m4d::transpose(modelM);
//        logMessage("%s %f, %f, %f\n", mObjMeshes[0]->getName().c_str(), pBody->getWorldTransform().m_origin[0], pBody->getWorldTransform().m_origin[1], pBody->getWorldTransform().m_origin[2]);
    }else {
        if(mTransform){ modelM = mTransform->transformMatrix(); }
    }

    
    Camera::instance()->pushMVMatrix(Camera::instance()->modelViewMatrix() * modelM);
    for (auto const& mesh : mObjMeshes) {
//        if(!Camera::instance()->sphereDistanceInFrustum(&mTransform->mPosition, mesh->radius)){
////            logMessage("skip %s mesh\n", mesh->getName().c_str());
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
        if(min.x > mesh->min.x) min.x = mesh->min.x;
        if(min.y > mesh->min.y) min.y = mesh->min.y;
        if(min.z > mesh->min.z) min.z = mesh->min.z;
        if(max.x < mesh->max.x) max.x = mesh->max.x;
        if(max.y < mesh->max.y) max.y = mesh->max.y;
        if(max.z < mesh->max.z) max.z = mesh->max.z;
    }
    v3d::print((max - min));
    
    return (max - min);
}