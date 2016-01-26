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
}


void MeshRendererComponent::draw(){
    m4d modelM = go->transformMatrix();
    Camera::instance()->pushMMatrix(modelM);
    for (auto const& mesh : mMeshes) {

        if(mesh){
            if(!Camera::instance()->sphereDistanceInFrustum(go->getPosition(), mesh->outlines.radius)){
                continue;
            }
//            modelM =  m4d::translate(mesh->outlines.location);
//            Camera::instance()->pushMVMatrix(modelM);
            
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

v3d MeshRendererComponent::getDimensions(){
    v3d dimensions, currMin, currMax;
    for(const auto& mesh : mMeshes){
        v3d min = mesh->outlines.min;
        if(currMin.x > min.x) currMin.x = min.x;
        if(currMin.y > min.y) currMin.y = min.y;
        if(currMin.z > min.z) currMin.z = min.z;
        
        v3d max = mesh->outlines.max;
        if(currMax.x < max.x) currMax.x = max.x;
        if(currMax.y < max.y) currMax.y = max.y;
        if(currMax.z < max.z) currMax.z = max.z;
    }
    dimensions = currMax - currMin;
    return dimensions;
}
