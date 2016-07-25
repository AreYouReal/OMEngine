#pragma once

#include "IComponent.hpp"
#include "md5.hpp"

class AnimMeshComponent : public IComponent{
    
public:
    enum AnimationStates{ NONE, IDLE, RUN, JUMP, FALL };

    
    AnimMeshComponent(GameObject* const go, sp<md5::MD5> md5Obj);
    
    virtual         ~AnimMeshComponent();
    
    virtual void    update() override;
    virtual void    draw() override;
    virtual void    onDestroy() override;
    
    void setState(AnimationStates state, bool loop);
    
    bool visible = true;
    
    v3d dimension(){ return md5->dimension; };
    
    AnimationStates getState(){ return mState; };

    bool castShadow = true;
    
    void setShadowDraw(const bool cast){ md5->shadowDraw = cast; }
    
private:
    sp<md5::MD5>    md5;
    
    AnimationStates mState{AnimationStates::IDLE};
    
    std::map<AnimationStates, string> stateActionTable;
    
    void updateAnimation(bool loop);
};