#pragma once

#include "Shortcuts.h"

#include "ASceneNode.hpp"



class Scene{
public:
    
    static Scene *instance();
    
    Scene();
    ~Scene();
    
    void addChild(ASceneNode *newNode);
    
    void update();
    
    void        setRenderObjectState(RenderObjectType newState);
    
private:
    
    RenderObjectType  mDrawingState = RenderObjectType::SOLID;
    

    std::vector<ASceneNode*> mRoots;
};
