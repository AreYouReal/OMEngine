#pragma once

#include "IComponent.hpp"
#include "md5.hpp"

class AnimMeshComponent : public IComponent{
    
public:
    enum AnimationStates{ NONE, IDLE, RUN, JUMP, FALL };

    
    AnimMeshComponent(GameObject* const go, const string meshName, const string materialname, std::vector<string> &actionsToLoad);
    
    virtual         ~AnimMeshComponent();
    
    virtual bool    init() override;
    virtual void    update() override;
    virtual void    draw() override;
    virtual void    onDestroy() override;
    
    void setState(AnimationStates state);
    
    bool visible = true;
    
    v3d dimension(){ return md5->dimension; };
    
    AnimationStates getState(){ return mState; };

private:
    sp<md5::MD5>    md5;
    
    AnimationStates mState{AnimationStates::IDLE};
    
    void updateAnimation();
};