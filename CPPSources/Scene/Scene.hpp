#pragma once

#include "Shortcuts.h"
#include "IRenderable.h"
#include "Singleton.hpp"
#include "GameObject.hpp"

#include "PhysicalWorld.hpp"
#include "Illuminator.hpp"


class Scene : public Singleton<Scene>{

public:
    
    Scene();
    Scene(const Scene& rhs) = delete;
    Scene& operator=(const Scene& rhs) = delete;
    
    ~Scene();
    
    void addObjOnScene(up<GameObject> go);
    void update(float deltaTime);
    void draw();
    void setRenderObjectState(RenderObjectType newState);
    
// DEBUG
    void createMOMO();
    
private:

// Fields
    RenderObjectType  mDrawingState = RenderObjectType::SOLID;

    std::vector<up<GameObject>> mObjects;
    
    std::map<string, sp<Obj>> mObjRess;
    
//Helpers
    
    void createCamera();
    void createIlluminator();
    void createMaterials();
    
// DEbug
    void createTestScene(sp<Obj> object);
    
};
