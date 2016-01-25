#include "MeshRendererComponent.hpp"

#include "WiredCube.hpp"
#include "btBoxShape.h"

MeshRendererComponent::MeshRendererComponent(GameObject * const gameObject): IComponent(gameObject){
    logMessage("MeshRendererComponent constructor!\n");
}

MeshRendererComponent::MeshRendererComponent( GameObject * const gameObject, const sp<ObjMesh> mesh) : IComponent(gameObject){
    mMeshes.push_back(mesh);
}

MeshRendererComponent::MeshRendererComponent(GameObject * const gameObject, const std::vector<sp<ObjMesh>> &meshes) : IComponent(gameObject){
    mMeshes = meshes;
    logMessage("MeshRendererComponent constructor with meshes!\n");
}

MeshRendererComponent::~MeshRendererComponent(){
    onDestroy();
    logMessage("MeshRendererComponent destructor!\n");
}


bool MeshRendererComponent::init(){
    return true;
}


void MeshRendererComponent::update(){
    m4d modelM = go->transformMatrix();
    Camera::instance()->pushMMatrix(modelM);
    for (auto const& mesh : mMeshes) {

        if(mesh){
//            logMessage("Name: %s\n", mesh->getName().c_str() );
            if(!Camera::instance()->sphereDistanceInFrustum(go->getPosition(), mesh->outlines.radius)){
                //            logMessage("skip %s mesh\n", mesh->getName().c_str());
                continue;
            }
//            modelM =  m4d::translate(mesh->outlines.location);
//            Camera::instance()->pushMVMatrix(modelM);
            
            if(mesh->renderObjectType() == RenderObjectType::SOLID){
//                if(Game::debugFlag){ drawDebugPhysicsGeometry(); }
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
         
            
//            Camera::instance()->popMVMatrix();
        }
    }
    
    for(auto const &child : go->mChildren){
        IComponent *meshRendererComp = child->getComponent(ComponentEnum::MESH_RENDERER);
        if(meshRendererComp){
            meshRendererComp->update();
        }
    }
    Camera::instance()->popMMatrix();
}


void MeshRendererComponent::onDestroy(){
    
}

void MeshRendererComponent::addMesh(sp<ObjMesh> mesh){
    mMeshes.push_back(mesh);
}


