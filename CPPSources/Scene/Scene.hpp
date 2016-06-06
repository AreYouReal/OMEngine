#pragma once

#include "Shortcuts.h"
#include "Singleton.hpp"
#include "GameObject.hpp"
#include "Illuminator.hpp"

#include "PhysicalWorld.hpp"

#include "Boombox.hpp"


#include "MeshRendererComponent.hpp"
#include "DebugDrawComponent.hpp"
#include "AnimMeshComponent.hpp"

#include "PlayerController.hpp"

class Scene : public Singleton<Scene>{

public:
    
    Scene();
    Scene(const Scene& rhs) = delete;
    Scene& operator=(const Scene& rhs) = delete;
    
    ~Scene();
    
    bool init();
    
    void addObjOnScene(up<GameObject> go);
    bool removeObjectFromTheScene(GameObject *go);
    void update(float deltaTime);
    void drawDepth();
    void draw();
    void setRenderObjectState(RenderObjectType newState);
    
    void touchBegin(const int x, const int y);
    
    
    
    
private:

// Fields
    RenderObjectType  mDrawingState = RenderObjectType::SOLID;
    std::vector<up<GameObject>> mObjects;
    std::map<string, sp<Obj>> mObjRess;
    
//Helpers
    void addLight();
    
    void loadBlockObj();
    void loadArrowObj();
    
    PlayerController *createPlayer();
    LevelBuilder *createLevelBuilder();
    
};
