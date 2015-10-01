#pragma once

#include "ASceneNode.hpp"

class Scene{
public:
    
    static Scene *instance();
    
    Scene();
    ~Scene();
    
    void addNode(ASceneNode *newNode);
    
    void update();
    
private:

    ASceneNode *mRoot;
};
