#include "Scene.hpp"
#include "Camera.h"
#include "Shortcuts.h"
#include "LevelBuilder.hpp"
#include "CandyMonster.hpp"
#include "MonsterSelector.hpp"
#include "PlayButton.hpp"



PlayerController *player    = nullptr;
LevelBuilder *lBuilder      = nullptr;
MonsterSelector *mSelector  = nullptr;
PlayButton      *playButton = nullptr;

#pragma Constr/Destr
Scene::Scene(){
    logGLError();
    AssetManager::instance()->init();
    Camera::instance();
    Materials::instance();
    Illuminator::instance();
    PhysicalWorld::instance();
    Boombox::instance();
    logGLError();
}

Scene::~Scene(){
    mObjects.clear();
    Camera::destroy();
    Illuminator::destroy();
    Materials::destroy();
    PhysicalWorld::destroy();
    Boombox::destroy();
}

#pragma OME event functions

bool Scene::init(){
    srand(time(0));
    
    logGLError();
    Camera::instance()->initShadowBuffer();

    
    createCandyMonsters();
    
    lBuilder = createLevelBuilder();
    
    addLight();
    addPlayButton();
    
    Camera::instance()->setPosition(v3d(0, 11, 5));
    Camera::instance()->setFront( v3d(0, 9, 0));
    
    return true;
}

void Scene::update(float deltaTime){
    Camera::instance()->update();
    PhysicalWorld::instance()->update(deltaTime);
    for(int i = 0; i < mObjects.size(); ++i){
        mObjects[i]->update();
    }

}

void Scene::draw(){
    
    logGLError();
    glBindFramebuffer(GL_FRAMEBUFFER, Camera::instance()->mMainBuffer);
    glColorMask ( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
    glViewport(0, 0, Camera::instance()->width(), Camera::instance()->height());
    glClearColor(Camera::instance()->mClearColor.x, Camera::instance()->mClearColor.y, Camera::instance()->mClearColor.z, Camera::instance()->mClearColor.w);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
    
    
    for(const auto& go : mObjects){
        go->draw();
    }
    
    logGLError();
}

void Scene::drawDepth(){
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &Camera::instance()->mMainBuffer);
    glBindFramebuffer( GL_FRAMEBUFFER, Camera::instance()->shadowBuffer());
    //        glBindFramebuffer(GL_FRAMEBUFFER, Camera::instance()->mMainBuffer);
    
    glViewport(0, 0, Camera::instance()->shadowmapWidth(), Camera::instance()->shadowmapHeight());
    glClear(GL_DEPTH_BUFFER_BIT);
    
    glColorMask ( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
    glEnable ( GL_POLYGON_OFFSET_FILL );
    glPolygonOffset( 5.0f, 100.0f );
    
    glCullFace(GL_FRONT);
    Camera::instance()->shadowDraw = true;
    for(const auto& go: mObjects){
        MeshRendererComponent *mrc = static_cast<MeshRendererComponent*>(go->getComponent(ComponentEnum::MESH_RENDERER));
        if(mrc && mrc->castShadows){
            mrc->shadowDraw = true;
            mrc->draw();
            mrc->shadowDraw = false;
        }
    }
    Camera::instance()->shadowDraw = false;
    
    glCullFace( GL_BACK );
    glDisable( GL_POLYGON_OFFSET_FILL );
}

void Scene::setRenderObjectState(RenderObjectType newState){
    if(mDrawingState != newState) mDrawingState = newState;
}

void Scene::onTouchBegin(const int x, const int y){
//    GameObject * collidedObj = Camera::instance()->collisionRayIntersection(x, y);
//    if(collidedObj != nullptr){
//        if(!collidedObj->name.compare("PLAY")){
//            if(player == nullptr){
//                player = createPlayer();
//                player->setLevelBuilder(lBuilder);
//                mSelector->go->mActive = false;
//            }
//            if(player){
//                player->onTouch();
//            }
//            playButton->go->mActive = false;
//        }
//    }else{
//        if(mSelector){
//            mSelector->onTouchBegin(x, y);
//        }
//        
//        if(player){
//            if(player->active()){
//                player->onTouch();
//            }
//        }
//    }
//    Camera::instance()->moveTo(v3d(0, 11, 15), 3.0f);

    
    Camera::instance()->rotateTo(220, v3d(0, 1, 0), 30.0f);
    
}

void Scene::onTouchMove(const int x, const int y){
    if(mSelector){
        mSelector->onTouchMove(x, y);
    }
}

void Scene::onTouchEnd(const int x, const int y){
    if(mSelector){
        mSelector->onTouchEnd(x, y);
    }
}

#pragma mark Init Helpers

void Scene::addPlayButton(){
    up<GameObject> playBTN = PlayButton::create();
    playButton = static_cast<PlayButton*>(playBTN->getComponent(ComponentEnum::PLAY_BTN));
    addObjOnScene(std::move(playBTN));
}

void Scene::addLight(){
    up<GameObject> go = std::unique_ptr<GameObject>(new GameObject("Light"));
    
    go->setFront(v3d(0, 1, -1));
    up<LightSource> light = up<LightSource>(new LightSource(go.get(), LightSource::Type::DIRECTION, v4d(1, 1, 1, 1)) );
    light->IComponent::mComponentType = ComponentEnum::LIGHT_SOURCE;
    go->addComponent(std::move(light));
    
    if(OMGame::debugFlag){
        up<DebugDrawComponent> debugDraw = up<DebugDrawComponent>(new DebugDrawComponent(go.get()));
        debugDraw->mComponentType = ComponentEnum::DEBUG_DRAW;
        go->addComponent(std::move(debugDraw));
    }
    
    addObjOnScene(std::move(go));
    
    go = std::unique_ptr<GameObject>(new GameObject("Light"));
    light = up<LightSource>(new LightSource(go.get(), LightSource::Type::POINT, v4d(1, 1, 1, 1), 50) );
//    go->setFront(v3d(5, 5, 0));
    // TODO: Why z = -z fro proper lighting calculations!?
    go->setPosition(v3d(0, 10, -3));
//    light->follow(player->go);
    light->IComponent::mComponentType = ComponentEnum::LIGHT_SOURCE;
    go->addComponent(std::move(light));

    if(OMGame::debugFlag){
        up<DebugDrawComponent> debugDraw = up<DebugDrawComponent>(new DebugDrawComponent(go.get()));
        debugDraw->mComponentType = ComponentEnum::DEBUG_DRAW;
        go->addComponent(std::move(debugDraw));
    }

    

    addObjOnScene(std::move(go));
}

PlayerController* Scene::createPlayer(){
    up<GameObject> candyMonster = CandyMonster::create(mSelector->getCurrentSelectedMonster());
    up<RigidBodyComponent> rbc_1 = up<RigidBodyComponent>(new RigidBodyComponent(candyMonster.get(), 5.0f));
    rbc_1->mBody->setGravity(btVector3(0, 0, 0));
    rbc_1->mComponentType = ComponentEnum::RIGID_BODY;
    candyMonster->addComponent(std::move(rbc_1));

    if(OMGame::debugFlag){
        up<DebugDrawComponent> ddc = up<DebugDrawComponent>(new DebugDrawComponent(candyMonster.get()));
        ddc->mComponentType = ComponentEnum::DEBUG_DRAW;
        candyMonster->addComponent(std::move(ddc));
    }
    candyMonster->mTransform.rotate(90, v3d(0, 1, 0));

    up<PlayerController> player = up<PlayerController>( new PlayerController(candyMonster.get()));
    PlayerController *ctr = player.get();
    ctr->IComponent::mComponentType = ComponentEnum::PLAYER_CTR;
    candyMonster->addComponent(std::move(player));
    
    addObjOnScene(std::move(candyMonster));
    
    return ctr;
}


void Scene::createCandyMonsters(){
    up<GameObject> monsterSelectorObject = up<GameObject>(new GameObject("MSelector"));    
    mSelector = MonsterSelector::add(monsterSelectorObject.get());
    
    for(int i =  1; i <= 5; ++i){
        up<GameObject> candyMonster = CandyMonster::create((CandyMonster::CandyType)i);
        mSelector->addMonster(std::move(candyMonster));
    }
    
    addObjOnScene(std::move(monsterSelectorObject));
}

LevelBuilder *Scene::createLevelBuilder(){
    up<GameObject> go = std::unique_ptr<GameObject>(new GameObject("LevelBuilder"));
    up<LevelBuilder> lb = std::unique_ptr<LevelBuilder>(new LevelBuilder(go.get()));
    lb->InitWithMeshes(AssetManager::instance()->getMeshFromObj("bblock.obj", "bblock"), AssetManager::instance()->getAllMeshesFromObj("candies.obj"));
    LevelBuilder *returnValue = lb.get();
    lb->IComponent::mComponentType = ComponentEnum::LEVEL_BUILDER;
    go->addComponent(std::move(lb));
    addObjOnScene(std::move(go));
    return returnValue;
}


#pragma mark Remove/Add object
void Scene::addObjOnScene(up<GameObject> go){
    for(auto const &comp : go->mComponents){
        comp.second->init();
    }
    mObjects.push_back(std::move(go));
}

bool Scene::removeObjectFromTheScene(GameObject *go){
    for(int i = 0; i < mObjects.size(); ++i){
        if(mObjects[i].get() == go){
            mObjects.erase(mObjects.begin() + i);
            return true;
        }
    }
    return false;
}