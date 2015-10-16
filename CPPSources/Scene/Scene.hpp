#pragma once

#include "Shortcuts.h"
#include "IRenderable.h"
#include "Singleton.hpp"
#include "ObjMesh.h"

class Scene : public Singleton<Scene>{

public:
    
    Scene();
    Scene(const Scene& rhs) = delete;
    Scene& operator=(const Scene& rhs) = delete;
    
    ~Scene();
    
    void addObjOnScene(sp<IRenderable> newNode);
    
    void update();
    
    void        setRenderObjectState(RenderObjectType newState);
    
private:
    
    RenderObjectType  mDrawingState = RenderObjectType::SOLID;
    

    std::vector<sp<IRenderable>> mRoots;
};
