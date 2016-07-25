#include "MeshRendererComponent.hpp"

#include "WiredCube.hpp"
#include "btBoxShape.h"

MeshRendererComponent::MeshRendererComponent(GameObject * const gameObject): IComponent(gameObject){
    mComponentType = ComponentEnum::MESH_RENDERER;
}

MeshRendererComponent::MeshRendererComponent( GameObject * const gameObject, const sp<ObjMesh> mesh) : IComponent(gameObject){
    mComponentType = ComponentEnum::MESH_RENDERER;
    mMeshes.push_back(mesh);
}

MeshRendererComponent::MeshRendererComponent(GameObject * const gameObject, const std::vector<sp<ObjMesh>> &meshes) : IComponent(gameObject){
    mComponentType = ComponentEnum::MESH_RENDERER;
    mMeshes = meshes;
    logMessage("MeshRendererComponent constructor with meshes!\n");
}

MeshRendererComponent::~MeshRendererComponent(){
    onDestroy();
}

void MeshRendererComponent::draw(){
    m4d modelM = go->transformMatrix();
    Camera::instance()->pushMMatrix(modelM);
    if(visible)
    for (auto const& mesh : mMeshes) {
        
        if(mesh){
            mesh->shadowDraw = shadowDraw;
            if(!Camera::instance()->sphereDistanceInFrustum(go->getPosition(), mesh->outlines.radius)){
//                logMessage("Renderer: Skip object: %s, [%f, %f, %f]\n", go->name.c_str(), go->getPosition().x, go->getPosition().y, go->getPosition().z);
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
    Camera::instance()->popMMatrix();
}


void MeshRendererComponent::onDestroy(){
    logMessage("Destroy mesh renderer component!");
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

v3d MeshRendererComponent::getPosition(){
    v3d pos;
    for(const auto& mesh : mMeshes){
        pos += mesh->outlines.location;
    }
    pos = 1.0f/mMeshes.size() * pos;
    return pos;
}

