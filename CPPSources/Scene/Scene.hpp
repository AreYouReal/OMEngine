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
    
    enum State{ START_VIEW, SELECT_MONSTER_VIEW, LEVEL_VIEW };
    
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
    
    void switchState(Scene::State newState);
    
private:

// Fields
    RenderObjectType  mDrawingState = RenderObjectType::SOLID;
    std::vector<up<GameObject>> mObjects;
    
    State       mState{START_VIEW};
    
//Helpers
    void addPlayButton();
    void addLight();
    
    PlayerController *createPlayer();
    LevelBuilder *createLevelBuilder();
    
    void startViewRoutine();
    void selectMonsterRoutine();
    void levelRoutine();
    
    
    void showPlayButton();
    void hidePlayButton();
    
};
