
#include "AnimMeshComponent.hpp"

#include "OMGame.h"

#include "Camera.h"

#include "GameObject.hpp"

AnimMeshComponent::AnimMeshComponent(GameObject* const go, const string meshName, const string materialname, std::vector<string> &actionsToLoad) : IComponent(go){
    
    Materials::instance()->loadMaterial(materialname);
    
    md5 = md5::MD5::loadMesh(meshName);
    
    initAnimMesh();
    
    for(auto const &action : actionsToLoad){
        std::size_t pos = action.find(".md5anim");
        string actionName = action.substr(0, pos);
        if(actionName.find("idle") != string::npos){
            stateActionTable.insert(std::pair<AnimationStates, string>(AnimationStates::IDLE, actionName));
        }else if(actionName.find("run") != string::npos){
            stateActionTable.insert(std::pair<AnimationStates, string>(AnimationStates::RUN, actionName));
        }else if(actionName.find("jump") != string::npos){
            stateActionTable.insert(std::pair<AnimationStates, string>(AnimationStates::JUMP, actionName));
        }
        md5->loadAction(actionName, action);
    }
    
    md5->mAnimType = md5::MD5::AnimType::SINGLE_ACTION;

//    md5->playAction("idle", md5::Action::InterpolationMethod::FRAME);
    md5->playAction(md5->actions.begin()->first, md5::Action::InterpolationMethod::FRAME);

}

AnimMeshComponent::~AnimMeshComponent(){
    onDestroy();
}

bool AnimMeshComponent::initAnimMesh(){
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
