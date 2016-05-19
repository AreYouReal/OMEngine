
#include "AnimMeshComponent.hpp"

#include "OMGame.h"

#include "Camera.h"

#include "GameObject.hpp"

AnimMeshComponent::AnimMeshComponent(GameObject* const go, const string meshName, const string materialname, std::vector<string> &actionsToLoad) : IComponent(go){
    
    md5 = md5::MD5::loadMesh(meshName);
    
    init();
    
    for(auto const &action : actionsToLoad){
        std::size_t pos = action.find(".md5anim");
        string actionName = action.substr(0, pos);
        md5->loadAction(actionName, action);
    }
    
    md5->mAnimType = md5::MD5::AnimType::SINGLE_ACTION;

//    md5->playAction("idle", md5::Action::InterpolationMethod::FRAME);
    md5->playAction(md5->actions.begin()->first, md5::Action::InterpolationMethod::FRAME);

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
    if(!visible) return;
    
    m4d modelM = go->transformMatrix();
    Camera::instance()->pushMMatrix(modelM);
    md5->draw();
    Camera::instance()->popMMatrix();
}

void AnimMeshComponent::onDestroy(){
}

void AnimMeshComponent::setState(AnimMeshComponent::AnimationStates state){
    logMessage("Set state: %d\n", (int)state);
    switch (state) {
        case AnimationStates::NONE:
            md5->stopAllActions();
            break;
        case AnimationStates::IDLE:
            md5->playAction("bob_idle", md5::Action::InterpolationMethod::FRAME);
            break;
        case AnimationStates::RUN:
            md5->playAction("bob_walk", md5::Action::InterpolationMethod::FRAME);
            break;
        case AnimationStates::JUMP:
            md5->playAction("jump", md5::Action::InterpolationMethod::FRAME);
            break;
        default:
            logMessage("Unknown animatino state!");
            md5->stopAllActions();
            break;
    }
 
}