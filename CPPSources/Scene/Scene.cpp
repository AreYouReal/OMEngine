#include "Scene.hpp"
#include "Camera.h"
#include "Shortcuts.h"
#include "PlayerController.hpp"
#include "LevelBuilder.hpp"

GameObject *minimon;
PlayerController *player;
LevelBuilder *lBuilder;

Scene::Scene(){
    logGLError();
    Camera::instance();
    Materials::instance();
    logGLError();
    Illuminator::instance();
    logGLError();

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

bool Scene::init(){
    logGLError();
    
//    createBallsScene();
//    createTestScene();
    

//    createLightTestScene();
    createBob();
    logGLError();
//    sp<Texture> projTexture = Materials::instance()->getTexture("projector.png");
//    glBindTexture(GL_TEXTURE_2D, projTexture->ID);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    logGLError();
    
    Camera::instance()->initShadowBuffer();
    
    logGLError();
    
    player = new PlayerController(minimon);
    Camera::instance()->follow(minimon, v3d(-5, 5, 5));

    up<GameObject> go = std::unique_ptr<GameObject>(new GameObject("LevelBuilder"));
    up<LevelBuilder> lb = std::unique_ptr<LevelBuilder>(new LevelBuilder(go.get()));
    lBuilder = lb.get();
    go->addComponent(ComponentEnum::LEVEL_BUILDER, std::move(lb));
    lBuilder->InitWithMeshes(mObjRess["bblock"]->getMesh("bblock_Cube"), mObjRess["arrow"]->getMesh("ArrowObj_Plane"));
    
    lBuilder->buildLevel();
    
    player->init(lBuilder);
    addObjOnScene(std::move(go));
    
    
    return true;
}

void Scene::addObjOnScene(up<GameObject> go){
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

void Scene::update(float deltaTime){
    Camera::instance()->update();
    PhysicalWorld::instance()->update(deltaTime);
    for(const auto& go : mObjects){
        for(auto const &comp : go->mComponents){
            comp.second->update();
        }
    }
        
    Illuminator::instance()->update(deltaTime);
    player->update();
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

void Scene::draw(){
   
    glBindFramebuffer(GL_FRAMEBUFFER, Camera::instance()->mMainBuffer);
    
    glColorMask ( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
    logGLError();
    
    glViewport(0, 0, Camera::instance()->width(), Camera::instance()->height());
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
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
    
//    Illuminator::instance()->getLightSource()->draw();
}

void Scene::setRenderObjectState(RenderObjectType newState){
    if(mDrawingState != newState) mDrawingState = newState;
}

void Scene::touchBegin(const int x, const int y){
    v3d farPlaneCoord = Camera::instance()->farPlanePoint(v3d(x, y, 1.0f));
    Camera::instance()->collisionRay(farPlaneCoord);
    if(player){
        player->onTouch();
    }
}

// DEBUG AND TEST STUFF GOES HERE

void Scene::createBallsScene(){
    
    sp<Obj> object = Obj::load("Scene.obj");
    object->build();
    object->clear();
    
    mObjRess.insert(std::pair<string, sp<Obj>>("ballsScene", object));
    
    up<GameObject> interior = std::unique_ptr<GameObject>(new GameObject("interior"));
    up<MeshRendererComponent> mrc = up<MeshRendererComponent>(new MeshRendererComponent(interior.get(), mObjRess["ballsScene"]->getMesh(interior->name)));
    interior->addComponent(ComponentEnum::MESH_RENDERER, std::move(mrc));
    
    up<GameObject> camera = std::unique_ptr<GameObject>(new GameObject("player"));
    mrc = up<MeshRendererComponent>(new MeshRendererComponent(camera.get(), mObjRess["ballsScene"]->getMesh(camera->name)));
    camera->addComponent(ComponentEnum::MESH_RENDERER, std::move(mrc));
    
    std::vector<GameObject*> objs;
    objs.push_back(interior.get());
    objs.push_back(camera.get());
    
    PhysicalWorld::instance()->loadPhysicsWorldFromFile("Scene.bullet", objs);
    
    addObjOnScene(std::move(interior));
    addObjOnScene(std::move(camera));
    
}

void Scene::addMeshRendererOnScene(string objName, string meshName){
    up<GameObject> go = std::unique_ptr<GameObject>(new GameObject(meshName));
    sp<ObjMesh> mesh =mObjRess[objName]->getMesh(go->name);
    if(strstr(meshName.c_str(), "sphere2")){
        mesh->tLists[0]->mode = GL_POINTS;
        go->mTransform = v3d(0, 0, 3);
    }
    

    
    up<MeshRendererComponent> mrc = up<MeshRendererComponent>(new MeshRendererComponent(go.get(), mesh));
    if(strstr(meshName.c_str(), "sphere3")){
        mrc->castShadows = true;
    }
    go->addComponent(ComponentEnum::MESH_RENDERER, std::move(mrc));
    

    
    addObjOnScene(std::move(go));
}

void Scene::createLightTestScene(){
logGLError();
    sp<Obj> object = Obj::load("Scene.obj");
    object->build();
    object->clear();
    
    mObjRess.insert(std::pair<string, sp<Obj>>("lightScene", object));
    
    logGLError();
    
    std::vector<sp<ObjMesh>> meshes = object->getAllMeshes();
    
    for(const sp<ObjMesh> mesh : object->getAllMeshes()){
        if(!mesh->getName().compare("projector") ) continue;
        addMeshRendererOnScene("lightScene", mesh->getName());
    }
}

void Scene::createBob(){

    sp<Obj> bblockObj = Obj::load("bblock.obj");
    bblockObj->build();
    bblockObj->clear();
    
    mObjRess.insert(std::pair<string, sp<Obj>>("bblock", bblockObj));
    
    sp<Obj> arrowObj = Obj::load("arrow.obj");
    arrowObj->build();
    arrowObj->clear();
    
    mObjRess.insert(std::pair<string, sp<Obj>>("arrow", arrowObj));
    
    
    up<GameObject> monster = std::unique_ptr<GameObject>(new GameObject("MONSTER"));
    std::vector<string>  monsterActions;
    monsterActions.clear();
    monsterActions.push_back("minimon_run.md5anim");
    
    minimon = monster.get();
    
    up<AnimMeshComponent> mamc = up<AnimMeshComponent>(new AnimMeshComponent(monster.get(), "minimon.md5mesh", "minimon.mtl", monsterActions));
    monster->addComponent(ComponentEnum::ANIM_MESH, std::move(mamc));
    
    up<RigidBodyComponent> rbc_1 = up<RigidBodyComponent>(new RigidBodyComponent(monster.get(), 5.0f));
    monster->addComponent(ComponentEnum::RIGID_BODY, std::move(rbc_1));
    
//    up<DebugDrawComponent> ddc = up<DebugDrawComponent>(new DebugDrawComponent(monster.get()));
//    monster->addComponent(ComponentEnum::DEBUG_DRAW, std::move(ddc));
    
//    monster->mTransform = (v3d(0, 0, 3));
    monster->mTransform.mScale = v3d(3, 3, 3);
    monster->mTransform.refreshTransformMatrix();
    q4d sR(90, v3d(0, 1, 0));
    monster->mTransform.rotate(sR);
    
    addObjOnScene(std::move(monster));

}