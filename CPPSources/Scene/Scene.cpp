#include "Scene.hpp"
#include "Camera.h"
#include "Shortcuts.h"
#include "LevelBuilder.hpp"


const std::string actionObjName{"candy.obj"};
const std::string bblockObjName{"bblock.obj"};

const string actionArrowMeshName{"candy"};
const string bblockMeshName{"bblock"};

PlayerController *player;

LevelBuilder *lBuilder;

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
    logGLError();
    Camera::instance()->initShadowBuffer();
    
   
    
    loadBlockObj();
    loadArrowObj();
    
    player = createPlayer();
    lBuilder = createLevelBuilder();
    player->setLevelBuilder(lBuilder);
    
    
     addLight();


    return true;
}

void Scene::update(float deltaTime){
    Camera::instance()->update();
    PhysicalWorld::instance()->update(deltaTime);
    for(int i = 0; i < mObjects.size(); ++i){
        for(auto const &comp : mObjects[i]->mComponents){
            comp.second->update();
        }
    }
}

void Scene::draw(){
    
    logGLError();
    glBindFramebuffer(GL_FRAMEBUFFER, Camera::instance()->mMainBuffer);
    glColorMask ( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
    glViewport(0, 0, Camera::instance()->width(), Camera::instance()->height());
    glClearColor(0.35f, 0.75f, 1.0f, 1.0f);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
    
    
    for(const auto& go : mObjects){
        for(int i = (int)ComponentEnum::MESH_RENDERER; i <= (int)ComponentEnum::DEBUG_DRAW; ++i){
            IComponent *comp = go->getComponent((ComponentEnum)i);
            if(comp){
                //                sp<Texture> projTexture = Materials::instance()->getTexture("projector.png");
                //                glActiveTexture(GL_TEXTURE0);
                //                glBindTexture(GL_TEXTURE_2D, Camera::instance()->depthTexture());
                comp->draw();
            }
        }
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

void Scene::touchBegin(const int x, const int y){   
    GameObject * collidedObj = Camera::instance()->collisionRayIntersection(x, y);
    if(collidedObj != nullptr){
        logMessage("Collided object! : %s\n", collidedObj->name.c_str());
    }

    if(player){
        player->onTouch();
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
    go->addComponent(ComponentEnum::LIGHT_SOURCE, std::move(light));
    
    up<DebugDrawComponent> debugDraw = up<DebugDrawComponent>(new DebugDrawComponent(go.get()));
    go->addComponent(ComponentEnum::DEBUG_DRAW, std::move(debugDraw));
    
    addObjOnScene(std::move(go));
    
    go = std::unique_ptr<GameObject>(new GameObject("Light"));
    light = up<LightSource>(new LightSource(go.get(), LightSource::Type::POINT, v4d(1, 1, 1, 1), 15) );
    go->setFront(v3d(5, 5, 0));
    light->follow(player->go);
    go->addComponent(ComponentEnum::LIGHT_SOURCE, std::move(light));
    debugDraw = up<DebugDrawComponent>(new DebugDrawComponent(go.get()));
    go->addComponent(ComponentEnum::DEBUG_DRAW, std::move(debugDraw));
    debugDraw = up<DebugDrawComponent>(new DebugDrawComponent(go.get()));
    go->addComponent(ComponentEnum::DEBUG_DRAW, std::move(debugDraw));
    addObjOnScene(std::move(go));
}

PlayerController* Scene::createPlayer(){
    up<GameObject> monster = std::unique_ptr<GameObject>(new GameObject("MONSTER"));
    std::vector<string>  monsterActions;
    monsterActions.clear();
    monsterActions.push_back("minimon_idle.md5anim");
    monsterActions.push_back("minimon_run.md5anim");
    monsterActions.push_back("minimon_jump_1.md5anim");
    monsterActions.push_back("minimon_jump_2.md5anim");
    
    up<AnimMeshComponent> mamc = up<AnimMeshComponent>(new AnimMeshComponent(monster.get(), "minimon.md5mesh", "minimon.mtl", monsterActions));
    monster->addComponent(ComponentEnum::ANIM_MESH, std::move(mamc));


    q4d sR(90, v3d(0, 1, 0));
    monster->mTransform.rotate(sR);
    monster->mTransform.translate(0, -.85f, 0);
    monster->mTransform.mScale = v3d(3, 3, 3);
    monster->mTransform.refreshTransformMatrix();
    
    up<RigidBodyComponent> rbc_1 = up<RigidBodyComponent>(new RigidBodyComponent(monster.get(), 5.0f));
    monster->addComponent(ComponentEnum::RIGID_BODY, std::move(rbc_1));
    
//    up<DebugDrawComponent> ddc = up<DebugDrawComponent>(new DebugDrawComponent(monster.get()));
//    monster->addComponent(ComponentEnum::DEBUG_DRAW, std::move(ddc));
    
    
    
    up<PlayerController> player = up<PlayerController>( new PlayerController(monster.get()));
    PlayerController *ctr = player.get();
    monster->addComponent(ComponentEnum::PLAYER_CTR, std::move(player));
    
    addObjOnScene(std::move(monster));

    return ctr;
}

LevelBuilder *Scene::createLevelBuilder(){
    up<GameObject> go = std::unique_ptr<GameObject>(new GameObject("LevelBuilder"));
    up<LevelBuilder> lb = std::unique_ptr<LevelBuilder>(new LevelBuilder(go.get()));
    lb->InitWithMeshes(mObjRess[bblockObjName]->getMesh(bblockMeshName), mObjRess[actionObjName]->getMesh(actionArrowMeshName));
    LevelBuilder *returnValue = lb.get();
        go->addComponent(ComponentEnum::LEVEL_BUILDER, std::move(lb));
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