#include "MeshRendererComponent.hpp"


MeshRendererComponent::MeshRendererComponent(GameObject * const gameObject): IComponent(gameObject){
    logMessage("MeshRendererComponent constructor!\n");
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
    // O_o
    m4d ATTRIBUTE_ALIGNED16(modelM);
    if(go->pBody){
        go->pBody->getWorldTransform().getOpenGLMatrix(modelM.pointer());
        modelM = m4d::transpose(modelM);
        //        logMessage("%s %f, %f, %f\n", mObjMeshes[0]->getName().c_str(), pBody->getWorldTransform().m_origin[0], pBody->getWorldTransform().m_origin[1], pBody->getWorldTransform().m_origin[2]);
    }else {
        if(go->mTransform){ modelM = go->mTransform->transformMatrix(); }
        else{ logMessage("Something went wrong. There is no physical body or transform... "); }
    }
    
    
    Camera::instance()->pushMVMatrix(Camera::instance()->modelViewMatrix() * modelM);
    for (auto const& mesh : mMeshes) {
        if(mesh){
            logMessage("Name: %s\n", mesh->getName().c_str());
            if(!Camera::instance()->sphereDistanceInFrustum(go->getPosition(), mesh->outlines.radius)){
                //            logMessage("skip %s mesh\n", mesh->getName().c_str());
                continue;
            }
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
    
    for(auto const &child : go->mChildren){
        IComponent *meshRendererComp = child->getComponent("MeshRender");
        if(meshRendererComp){
            meshRendererComp->update();
        }
    }
    
    Camera::instance()->popMVMatrix();

}


void MeshRendererComponent::onDestroy(){
    
}

void MeshRendererComponent::addMesh(sp<ObjMesh> mesh){
    mMeshes.push_back(mesh);
}


