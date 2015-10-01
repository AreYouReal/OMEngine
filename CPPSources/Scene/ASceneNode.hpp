#pragma once

#include <vector>
#include "ObjMesh.h"
#include "Transform.hpp"

class ASceneNode{
public:
    ASceneNode(Transform *tr, ObjMesh *m);
    virtual ~ASceneNode();
    
    void release();
    
    void addChild(ASceneNode *child);
    void destroyChildren();
    
    virtual void update();

private:
    std::vector<ASceneNode*> mChildren;
    
    Transform   *transform;
    ObjMesh     *mesh;
};