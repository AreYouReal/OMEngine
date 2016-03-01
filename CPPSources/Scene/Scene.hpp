#pragma once

#include "Shortcuts.h"
#include "Singleton.hpp"
#include "GameObject.hpp"
#include "Illuminator.hpp"

#include "PhysicalWorld.hpp"

#include "Boombox.hpp"


#include "MeshRendererComponent.hpp"
#include "DebugDrawComponent.hpp"

class Scene : public Singleton<Scene>{

public:
    
    Scene();
    Scene(const Scene& rhs) = delete;
    Scene& operator=(const Scene& rhs) = delete;
    
    ~Scene();
    
    bool init();
    
    void addObjOnScene(up<GameObject> go);
    void update(float deltaTime);
    void drawDepth();
    void draw();
    void setRenderObjectState(RenderObjectType newState);
    
// DEBUG
    
    // Ground, bg, tree and momos...
    void createMOMO();
    void createTestScene();
    
    // Balls
    
    void createBallsScene();
    
    
    // Light test
    
    void createLightTestScene();
    
    void addMeshRendererOnScene(string objName, string meshName);
    
private:

// Fields
    RenderObjectType  mDrawingState = RenderObjectType::SOLID;

    std::vector<up<GameObject>> mObjects;
    
    std::map<string, sp<Obj>> mObjRess;
    
//Helpers
    
    
};
