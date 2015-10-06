#pragma once

#include "Shortcuts.h"

#include "ASceneNode.hpp"

class Scene : public Singleton<Scene>{

public:
    
    Scene();
    Scene(const Scene& rhs) = delete;
    Scene& operator=(const Scene& rhs) = delete;
    
    ~Scene();
    
    
    
    void addChild(ASceneNode *newNode);
    
    void update();
    
    void        setRenderObjectState(RenderObjectType newState);
    
private:
    
    RenderObjectType  mDrawingState = RenderObjectType::SOLID;
    

    std::vector<ASceneNode*> mRoots;
};
