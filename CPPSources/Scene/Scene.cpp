#include "Scene.hpp"
#include "Camera.h"
#include "Shortcuts.h"
#include "PlayerController.hpp"
#include "LevelBuilder.hpp"

GameObject *bob;
PlayerController *player;
Levelbuilder *lBuilder;

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
    
    player = new PlayerController(bob);
    Camera::instance()->follow(bob, v3d(-7, -7, 10));

    
    lBuilder = new Levelbuilder();
    lBuilder->mesh = mObjRess["bblock"]->getMesh("bblock_Cube");
    lBuilder->arrow = mObjRess["arrow"]->getMesh("ArrowObj_Plane");
    
    lBuilder->buildLevel(player->actions);
    
    player->debugInit();
    return true;
}

void Scene::addObjOnScene(up<GameObject> go){
    mObjects.push_back(std::move(go));
}

void Scene::update(float deltaTime){
    Camera::instance()->update();
    PhysicalWorld::instance()->update(deltaTime);
    for(const auto& go : mObjects){
        for(int i = (int)ComponentEnum::MESH_RENDERER; i <= (int)ComponentEnum::DEBUG_DRAW; ++i){
            IComponent *comp = go->getComponent((ComponentEnum)i);
            if(comp) comp->update();
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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
    
    Illuminator::instance()->getLightSource()->draw();
}

void Scene::setRenderObjectState(RenderObjectType newState){
    if(mDrawingState != newState) mDrawingState = newState;
}

void Scene::touchBegin(const int x, const int y){
    if(player){
        player->onTouch();
    }
}

// DEBUG AND TEST STUFF GOES HERE
void Scene::createTestScene(){
    
    sp<Obj> object = Obj::load("treemomo.obj");
    object->build();
    object->clear(); // Free mesh data.
    
    mObjRess.insert(std::pair<string, sp<Obj>>("scene", object));
    
    /// TEST CODE
    
    up<GameObject> ground;

    // Tree
    
    up<GameObject> treeAndLeafs = std::unique_ptr<GameObject>(new GameObject());
    
    up<MeshRendererComponent> mrc_1 = up<MeshRendererComponent>(new MeshRendererComponent(treeAndLeafs.get()));
    mrc_1->addMesh(object->getMesh("leaf"));
    mrc_1->addMesh(object->getMesh("tree"));
    
    treeAndLeafs->mTransform = (v3d(0, 0, 5));
    treeAndLeafs->addComponent(ComponentEnum::MESH_RENDERER, std::move(mrc_1));
    
    up<RigidBodyComponent> rbc_1 = up<RigidBodyComponent>(new RigidBodyComponent(treeAndLeafs.get()));
    treeAndLeafs->addComponent(ComponentEnum::RIGID_BODY, std::move(rbc_1));

    up<DebugDrawComponent> debugDraw = up<DebugDrawComponent>(new DebugDrawComponent(treeAndLeafs.get()));
    
    treeAndLeafs->addComponent(ComponentEnum::DEBUG_DRAW, std::move(debugDraw));
    
    
    //------------------------------
    addObjOnScene(std::move(treeAndLeafs));
    
    // MOMO
    up<GameObject> momo = std::unique_ptr<GameObject>(new GameObject());

    mrc_1 = up<MeshRendererComponent>(new MeshRendererComponent(momo.get()));
    mrc_1->addMesh(object->getMesh("momo"));
    
    momo->name = "momo";
    momo->mTransform = (v3d(1.3, 0, 7));
    momo->addComponent(ComponentEnum::MESH_RENDERER, std::move(mrc_1));
    
    up<RigidBodyComponent> rbc_2 = up<RigidBodyComponent>(new RigidBodyComponent(momo.get()));
    momo->addComponent(ComponentEnum::RIGID_BODY, std::move(rbc_2));

    debugDraw = up<DebugDrawComponent>(new DebugDrawComponent(momo.get()));
    
    momo->addComponent(ComponentEnum::DEBUG_DRAW, std::move(debugDraw));
    
    //----------------------------
    addObjOnScene(std::move(momo));
    
    // GROUND
    
    ground = std::unique_ptr<GameObject>(new GameObject());
    ground->mTransform = (v3d(0, 0, 0));

    
    mrc_1 = up<MeshRendererComponent>(new MeshRendererComponent(ground.get()));
    mrc_1->addMesh(object->getMesh("grass_ground"));
    
    ground->addComponent(ComponentEnum::MESH_RENDERER, std::move(mrc_1));
    
    up<RigidBodyComponent> rbc_3 = up<RigidBodyComponent>(new RigidBodyComponent(ground.get(), 0.0f));
    ground->addComponent(ComponentEnum::RIGID_BODY, std::move(rbc_3));
    
    debugDraw = up<DebugDrawComponent>(new DebugDrawComponent(ground.get()));
    
    ground->addComponent(ComponentEnum::DEBUG_DRAW, std::move(debugDraw));
    
    addObjOnScene(std::move(ground));
}




void Scene::createMOMO(){    
    up<GameObject> m = std::unique_ptr<GameObject>(new GameObject());
    m->mTransform = (v3d(2, 0, 10));
    
    up<MeshRendererComponent> mrc_1 = up<MeshRendererComponent>(new MeshRendererComponent(m.get()));
    mrc_1->addMesh(mObjRess["scene"]->getMesh("momo"));
    
    m->addComponent(ComponentEnum::MESH_RENDERER, std::move(mrc_1));
    
//    PhysicalWorld::instance()->addPBodyToGameObject(m.get(), PhysicalBodyShape::BOX, 1.0f, m->getDimensions());
    
    addObjOnScene(std::move(m));
}

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
    up<GameObject> go = std::unique_ptr<GameObject>(new GameObject("BOB"));
    std::vector<string> actions;
    actions.push_back("bob_walk.md5anim");
//    actions.push_back("bob_idle.md5anim");

    
    bob = go.get();
    
    up<AnimMeshComponent> amc = up<AnimMeshComponent>(new AnimMeshComponent(go.get(), "bob.md5mesh", "bob.mtl", actions));
    go->addComponent(ComponentEnum::ANIM_MESH, std::move(amc));
    go->mTransform = (v3d(0, 0, 1));
    
    up<RigidBodyComponent> rbc_1 = up<RigidBodyComponent>(new RigidBodyComponent(go.get(), 5.0f));
    go->addComponent(ComponentEnum::RIGID_BODY, std::move(rbc_1));
    
    up<DebugDrawComponent> ddc = up<DebugDrawComponent>(new DebugDrawComponent(go.get()));
    go->addComponent(ComponentEnum::DEBUG_DRAW, std::move(ddc));
    
    addObjOnScene(std::move(go));

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
    actions.clear();
    actions.push_back("monster.md5anim");
    
    up<AnimMeshComponent> mamc = up<AnimMeshComponent>(new AnimMeshComponent(monster.get(), "monster.md5mesh", "monster.mtl", actions));
    monster->addComponent(ComponentEnum::ANIM_MESH, std::move(mamc));
    monster->mTransform = (v3d(0, 0, 3));
    monster->mTransform.mScale = v3d(0.1, 0.1, 0.1);
    monster->mTransform.rotate(90, v3d(1, 0, 0));
    
    addObjOnScene(std::move(monster));

}