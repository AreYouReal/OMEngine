#pragma once

#include <vector>
#include "ObjMesh.h"
#include "Transform.hpp"

class ASceneNode{
public:
    ASceneNode(sp<Transform> tr, sp<ObjMesh> m);
    virtual ~ASceneNode();
    
    void release();
    
    void addChild(up<ASceneNode> child);
    void destroyChildren();
    
    virtual void update();

private:
    std::vector<up<ASceneNode>> mChildren;
    
    sp<Transform>   transform;
    sp<ObjMesh>     mesh;
};