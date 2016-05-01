#pragma once

#include "IComponent.hpp"
#include "md5.hpp"


class AnimMeshComponent : public IComponent{
public:
    AnimMeshComponent(GameObject* const go, const string meshName, const string materialname, std::vector<string> &actionsToLoad);
    
    virtual         ~AnimMeshComponent();
    
    virtual bool    init() override;
    virtual void    update() override;
    virtual void    draw() override;
    virtual void    onDestroy() override;
    
private:
    
    sp<md5::MD5>    md5;
};