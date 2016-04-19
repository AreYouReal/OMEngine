
#include "AnimMeshComponent.hpp"

#include "OMGame.h"

AnimMeshComponent::AnimMeshComponent(GameObject* const go, const string meshName, const string materialname, std::vector<string> &actionsToLoad) : IComponent(go){
    
    md5 = md5::MD5::loadMesh(meshName);
    
    init();
    
    md5->loadAction("walk", "bob_walk.md5anim");
    md5->loadAction("idle", "bob_idle.md5anim");
    
    md5->mAnimType = md5::MD5::AnimType::SINGLE_ACTION;

    md5->playAction("idle", md5::Action::InterpolationMethod::FRAME);
    md5->playAction("walk", md5::Action::InterpolationMethod::FRAME);

}

AnimMeshComponent::~AnimMeshComponent(){
    onDestroy();
}

bool AnimMeshComponent::init(){
    md5->optimize(128);
    md5->build();
    md5->freeMeshData();
    return true;
}

void AnimMeshComponent::update(){
    md5->updateActions(Time::deltaTime);
}

void AnimMeshComponent::draw(){
    md5->draw();
}

void AnimMeshComponent::onDestroy(){
}