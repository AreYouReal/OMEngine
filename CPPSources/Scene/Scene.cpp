#include "Scene.hpp"
#include "Camera.h"
#include "Shortcuts.h"
#include "LevelBuilder.hpp"
#include "CandyMonster.hpp"
#include "MonsterSelector.hpp"
#include "PlayButton.hpp"

PlayerController    *player    = nullptr;
LevelBuilder        *lBuilder      = nullptr;
MonsterSelector     *mSelector  = nullptr;
PlayButton          *playButton = nullptr;

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


#pragma Public
void Scene::switchState(Scene::State newState){
    switch (newState) {
        case Scene::State::START_VIEW:
            startViewRoutine();
            break;
        case Scene::State::SELECT_MONSTER_VIEW:
            selectMonsterRoutine();
            break;
        case Scene::State::LEVEL_VIEW:
            levelRoutine();
            break;
        default:
            break;
    }
    mState = newState;
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

#pragma OME event functions

bool Scene::init(){
    srand(time(0));
    
    logGLError();

    switchState(State::START_VIEW);
    
    Camera::instance()->initShadowBuffer();
    
    addLight();

    
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

    Camera::instance()->draw();
    for(const auto& go : mObjects){ go->draw(); }
    
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
    switch (mState) {
        case State::START_VIEW:{
            GameObject * collidedObj = Camera::instance()->collisionRayIntersection(x, y);
            if(collidedObj){
                if(!collidedObj->name.compare("PLAY")){
                    if(playButton->go->mActive)
                        switchState(LEVEL_VIEW);
                }
            }
            break;
        }
        case State::SELECT_MONSTER_VIEW:
            if(mSelector){
                mSelector->onTouchBegin(x, y);
            }
            break;
        case State::LEVEL_VIEW:
            if(player) player->onTouch();
            break;
        default:
            break;
    }
}

void Scene::onTouchMove(const int x, const int y){
    switch (mState) {
        case State::SELECT_MONSTER_VIEW:
            if(mSelector){
                mSelector->onTouchBegin(x, y);
            }
            break;
        default:
            break;
    }
}

void Scene::onTouchEnd(const int x, const int y){
    switch (mState) {
        case State::SELECT_MONSTER_VIEW:
            if(mSelector){
                mSelector->onTouchBegin(x, y);
            }
            break;
        default:
            break;
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
    up<GameObject> candyMonster = CandyMonster::create(CandyMonster::TYPE_1);
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

void Scene::startViewRoutine(){
    addPlayButton();
    showPlayButton();
    if(player == nullptr){
        player = createPlayer();
    }
    Camera::instance()->setPosition(v3d(0, 11, 5));
    Camera::instance()->lookAt( v3d(0, 9, 0));
}

void Scene::levelRoutine(){
     hidePlayButton();
    if(!lBuilder){
        up<GameObject> levelBuilderObject = LevelBuilder::create();
        lBuilder = static_cast<LevelBuilder*>( levelBuilderObject->getComponent(ComponentEnum::LEVEL_BUILDER) );

        player->setLevelBuilder(lBuilder);
        addObjOnScene(std::move(levelBuilderObject));
    }
    lBuilder->buildLevel();
    player->activate();
}

void Scene::selectMonsterRoutine(){
    if(!mSelector){
        up<GameObject> monsterSelectorObject = up<GameObject>(new GameObject("MSelector"));
        mSelector = MonsterSelector::add(monsterSelectorObject.get());
        
        for(int i =  1; i <= 5; ++i){
            up<GameObject> candyMonster = CandyMonster::create((CandyMonster::CandyType)i);
            mSelector->addMonster(std::move(candyMonster));
        }
        
        addObjOnScene(std::move(monsterSelectorObject));
    }
}

void Scene::showPlayButton(){
    playButton->go->mActive = true;
}

void Scene::hidePlayButton(){
    playButton->go->mActive = false;
}
