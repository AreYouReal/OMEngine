#pragma once

#include "Shortcuts.h"
#include "IRenderable.h"
#include "Singleton.hpp"
#include "GameObject.hpp"

#include "PhysicalWorld.hpp"
#include "Illuminator.hpp"

#include "MeshRendererComponent.hpp"

class Scene : public Singleton<Scene>{

public:
    
    Scene();
    Scene(const Scene& rhs) = delete;
    Scene& operator=(const Scene& rhs) = delete;
    
    ~Scene();
    
    bool init();
    
    void addObjOnScene(up<GameObject> go);
    void update(float deltaTime);
    void draw();
    void setRenderObjectState(RenderObjectType newState);
    
// DEBUG
    
    // Ground, bg, tree and momos...
    void createMOMO();
    void createTestScene();
    
    // Balls
    
    void createBallsScene();
    
    
private:

// Fields
    RenderObjectType  mDrawingState = RenderObjectType::SOLID;

    std::vector<up<GameObject>> mObjects;
    
    std::map<string, sp<Obj>> mObjRess;
    
//Helpers
    
    
};
