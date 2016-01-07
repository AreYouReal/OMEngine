#include "Scene.hpp"

Scene::Scene(){
    
    Camera::instance();
    Illuminator::instance();
    Materials::instance();
    PhysicalWorld::instance();
    Boombox::instance();
    
    
    logMessage("Scene constructor!\n");
}

Scene::~Scene(){
    mObjects.clear();

    Camera::destroy();
    Illuminator::destroy();
    Materials::destroy();
    PhysicalWorld::destroy();
    Boombox::destroy();
    
    
    logMessage("Scene destructor!\n");
}

bool Scene::init(){
//    createBallsScene();
    
        createTestScene();
    
//    createLightTestScene();

    return true;
}

void Scene::addObjOnScene(up<GameObject> go){
    mObjects.push_back(std::move(go));
}

void Scene::update(float deltaTime){
    PhysicalWorld::instance()->update(deltaTime);
}

void Scene::draw(){
    for(const auto& go : mObjects){
        IComponent *mrc = go->getComponent(ComponentEnum::MESH_RENDERER);
        if(mrc){
            if(!mrc->go->name.compare("player")){
                Camera::instance()->setFront(mrc->go->getPosition());
            }
            mrc->update();
        }
        
        IComponent *ddc = go->getComponent(ComponentEnum::DEBUG_DRAW);
        if(ddc){ ddc->update(); }
        
    }
    
    Illuminator::instance()->getLightSource()->draw();

}

void Scene::setRenderObjectState(RenderObjectType newState){
    if(mDrawingState != newState) mDrawingState = newState;
}

// DEBUG AND TEST STUFF GOES HERE


void Scene::createTestScene(){
    
    sp<Obj> object = Obj::load("treemomo.obj");
    object->build();
    object->clear(); // Free mesh data.
    
    mObjRess.insert(std::pair<string, sp<Obj>>("scene", object));
    
    /// TEST CODE
    v3d firstPos(0, 0, 0);
    
    up<GameObject> ground;
    
    
    // Tree
    
    up<GameObject> treeAndLeafs = std::unique_ptr<GameObject>(new GameObject());
    
    up<MeshRendererComponent> mrc_1 = up<MeshRendererComponent>(new MeshRendererComponent(treeAndLeafs.get()));
    mrc_1->addMesh(object->getMesh("leaf"));
    mrc_1->addMesh(object->getMesh("tree"));
    
    treeAndLeafs->mTransform = (v3d(0, 0, 5));
    treeAndLeafs->addComponent(ComponentEnum::MESH_RENDERER, std::move(mrc_1));
    
    PhysicalWorld::instance()->addPBodyToGameObject(treeAndLeafs.get(), PhysicalBodyShape::BOX, 1.0f, treeAndLeafs->getDimensions());
    
    //------------------------------
    addObjOnScene(std::move(treeAndLeafs));
    
    // MOMO
    up<GameObject> momo = std::unique_ptr<GameObject>(new GameObject());

    mrc_1 = up<MeshRendererComponent>(new MeshRendererComponent(momo.get()));
    mrc_1->addMesh(object->getMesh("momo"));
    
    momo->name = "momo";
    momo->mTransform = (v3d(2.3, 0, 7));
    momo->addComponent(ComponentEnum::MESH_RENDERER, std::move(mrc_1));
    
    
    PhysicalWorld::instance()->addPBodyToGameObject(momo.get(), PhysicalBodyShape::BOX, 1.0f, momo->getDimensions());
    
    //----------------------------
    addObjOnScene(std::move(momo));
    
    // GROUND
    
    ground = std::unique_ptr<GameObject>(new GameObject());
    ground->mTransform = (v3d(0, 0, 0));

    
    mrc_1 = up<MeshRendererComponent>(new MeshRendererComponent(ground.get()));
    mrc_1->addMesh(object->getMesh("grass_ground"));
    
    ground->addComponent(ComponentEnum::MESH_RENDERER, std::move(mrc_1));
    
    PhysicalWorld::instance()->addPBodyToGameObject(ground.get(), PhysicalBodyShape::BOX, 0.0f, v3d(10, 10, 1));
    
    //------------------------------------------------
    
    addObjOnScene(std::move(ground));
    
    
    
    //
    //    , nullptr, [](btBroadphasePair &pair, btCollisionDispatcher &dispatcher, const btDispatcherInfo &info){
    //        logMessage("nearCallback");
    //        GameObject *go = (GameObject*)((btRigidBody*)(pair.m_pProxy0->m_clientObject))->getUserPointer();
    //        if(go->name.compare("momo")) return;
    //
    //        dispatcher.defaultNearCallback(pair, dispatcher, info);
    //    }
}




void Scene::createMOMO(){    
    up<GameObject> m = std::unique_ptr<GameObject>(new GameObject());
    m->mTransform = (v3d(2, 0, 10));
    
    up<MeshRendererComponent> mrc_1 = up<MeshRendererComponent>(new MeshRendererComponent(m.get()));
    mrc_1->addMesh(mObjRess["scene"]->getMesh("momo"));
    
    m->addComponent(ComponentEnum::MESH_RENDERER, std::move(mrc_1));
    
    PhysicalWorld::instance()->addPBodyToGameObject(m.get(), PhysicalBodyShape::BOX, 1.0f, m->getDimensions());
    
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
    up<MeshRendererComponent> mrc = up<MeshRendererComponent>(new MeshRendererComponent(go.get(), mObjRess[objName]->getMesh(go->name)));
    go->addComponent(ComponentEnum::MESH_RENDERER, std::move(mrc));
    
    up<DebugDrawComponent> ddc = up<DebugDrawComponent>(new DebugDrawComponent(go.get(), v3d(rand() % 10, 2, 2)));
    go->addComponent(ComponentEnum::DEBUG_DRAW, std::move(ddc));
    
    addObjOnScene(std::move(go));
}

void Scene::createLightTestScene(){
    sp<Obj> object = Obj::load("Scene.obj");
    object->build();
    object->clear();
    
    mObjRess.insert(std::pair<string, sp<Obj>>("lightScene", object));
    
    std::vector<sp<ObjMesh>> meshes = object->getAllMeshes();
    
    for(const sp<ObjMesh> mesh : object->getAllMeshes()){
            addMeshRendererOnScene("lightScene", mesh->getName());
    }
}