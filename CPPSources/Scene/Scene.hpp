#pragma once

#include "ASceneNode.hpp"

class Scene : public Singleton<Scene>{
public:
    
    Scene();
    ~Scene();
    
    void addChild(ASceneNode *newNode);
    
    void update();
    
    void        setRenderObjectState(RenderObjectType newState);
    
private:
    
    RenderObjectType  mDrawingState = RenderObjectType::SOLID;
    

    std::vector<ASceneNode*> mRoots;
};
