#pragma once

#include "Shortcuts.h"
#include "Singleton.hpp"
#include "GameObject.hpp"
#include "Illuminator.hpp"
#include "AssetManager.hpp"
#include "PhysicalWorld.hpp"
#include "Boombox.hpp"

#include "DebugDrawComponent.hpp"

#include "PlayerController.hpp"

#include "OMGame.h"

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
    
    void onTouchBegin(const int x, const int y);
    void onTouchMove(const int x, const int y);
    void onTouchEnd(const int x, const int y);
    
private:

// Fields
    RenderObjectType  mDrawingState = RenderObjectType::SOLID;
    std::vector<up<GameObject>> mObjects;
    
//Helpers
    void addPlayButton();
    void addLight();
    
    PlayerController *createPlayer();
    LevelBuilder *createLevelBuilder();
    
    void createCandyMonsters();
    
};
