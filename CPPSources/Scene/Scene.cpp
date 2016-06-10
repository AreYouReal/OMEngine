#include "Scene.hpp"
#include "Camera.h"
#include "Shortcuts.h"
#include "LevelBuilder.hpp"
#include "CandyMonster.hpp"
#include "MonsterSelector.hpp"


const std::string actionObjName{"candies.obj"};
const std::string bblockObjName{"bblock.obj"};

const string actionArrowMeshName{"candy_1"};
const string bblockMeshName{"bblock"};

PlayerController *player    = nullptr;
LevelBuilder *lBuilder      = nullptr;
MonsterSelector *mSelector  = nullptr;

#pragma Constr/Destr
Scene::Scene(){
    logGLError();
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

    loadBlockObj();
    loadArrowObj();
    
    createCandyMonsters();
    
//    player = createPlayer();
//    lBuilder = createLevelBuilder();
//    player->setLevelBuilder(lBuilder);
    
    
     addLight();

    
    Camera::instance()->setPosition(v3d(0, 9, 6));

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
    GameObject * collidedObj = Camera::instance()->collisionRayIntersection(x, y);
    if(collidedObj != nullptr){
        logMessage("Collided object! : %s\n", collidedObj->name.c_str());
    }

    if(player){
        player->onTouch();
    }
    
    if(mSelector){
        mSelector->onTouchBegin(x, y);
    }
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
void Scene::loadBlockObj(){
    sp<Obj> bblockObj = Obj::load(bblockObjName.c_str());
    bblockObj->build();
    bblockObj->clear();
    
    mObjRess.insert(std::pair<string, sp<Obj>>(bblockObjName, bblockObj));
}

void Scene::loadArrowObj(){
    sp<Obj> arrowObj = Obj::load(actionObjName.c_str());
    arrowObj->build();
    arrowObj->clear();
    
    mObjRess.insert(std::pair<string, sp<Obj>>(actionObjName, arrowObj));
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
    light = up<LightSource>(new LightSource(go.get(), LightSource::Type::POINT, v4d(1, 1, 1, 1), 15) );
    go->setFront(v3d(5, 5, 0));
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
    
    up<GameObject> candyMonster = CandyMonster::create(CandyMonster::CandyType::TYPE_1);
    up<RigidBodyComponent> rbc_1 = up<RigidBodyComponent>(new RigidBodyComponent(candyMonster.get(), 5.0f));
    rbc_1->mBody->setGravity(btVector3(0, 0, 0));
    rbc_1->mComponentType = ComponentEnum::RIGID_BODY;
    candyMonster->addComponent(std::move(rbc_1));

    if(OMGame::debugFlag){
        up<DebugDrawComponent> ddc = up<DebugDrawComponent>(new DebugDrawComponent(candyMonster.get()));
        ddc->mComponentType = ComponentEnum::DEBUG_DRAW;
        candyMonster->addComponent(std::move(ddc));
    }

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
    lb->InitWithMeshes(mObjRess[bblockObjName]->getMesh(bblockMeshName), mObjRess[actionObjName]->getAllMeshes());
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