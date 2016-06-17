
#include "AnimMeshComponent.hpp"

#include "OMGame.h"

#include "Camera.h"

#include "GameObject.hpp"

#include "AssetManager.hpp"

AnimMeshComponent::AnimMeshComponent(GameObject* const go, sp<md5::MD5> md5Obj) : IComponent(go){

    md5 = md5Obj;
    
    for(auto const &action : md5->getAllActions()){
        string actionName = action.first;
        if(action.first.find("idle") != string::npos){
            stateActionTable.insert(std::pair<AnimationStates, string>(AnimationStates::IDLE, actionName));
        }else if(action.first.find("run") != string::npos){
            stateActionTable.insert(std::pair<AnimationStates, string>(AnimationStates::RUN, actionName));
        }else if(action.first.find("jump") != string::npos){
            stateActionTable.insert(std::pair<AnimationStates, string>(AnimationStates::JUMP, actionName));
        }
    }
    
    md5->mAnimType = md5::MD5::AnimType::SINGLE_ACTION;

    md5->playAction(md5->actions.begin()->first, md5::Action::InterpolationMethod::FRAME);
}

AnimMeshComponent::~AnimMeshComponent(){
    onDestroy();
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

void AnimMeshComponent::setState(AnimMeshComponent::AnimationStates state, bool loop){
    mState = state;
    updateAnimation(loop);
}

void AnimMeshComponent::updateAnimation(bool loop){
    switch (mState) {
        case AnimationStates::NONE:
            md5->stopAllActions();
            break;
        case AnimationStates::IDLE:
        case AnimationStates::RUN:
        case AnimationStates::JUMP:
            md5->playAction(stateActionTable[mState], md5::Action::InterpolationMethod::FRAME, loop);
            break;
        default:
            logMessage("Unknown animatino state!");
            md5->stopAllActions();
            break;
    }
}
