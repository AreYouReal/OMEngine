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
    for(const auto& go : mObjects){
        for(int i = (int)ComponentEnum::MESH_RENDERER; i <= (int)ComponentEnum::DEBUG_DRAW; ++i){
            IComponent *comp = go->getComponent((ComponentEnum)i);
            if(comp) comp->update();
        }
    }
}

void Scene::draw(){
    for(const auto& go : mObjects){
        for(int i = (int)ComponentEnum::MESH_RENDERER; i <= (int)ComponentEnum::DEBUG_DRAW; ++i){
            IComponent *comp = go->getComponent((ComponentEnum)i);
            if(comp) comp->draw();
        }
    }
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
    momo->mTransform = (v3d(2.3, 0, 7));
    momo->addComponent(ComponentEnum::MESH_RENDERER, std::move(mrc_1));
    
    up<RigidBodyComponent> rbc_2 = up<RigidBodyComponent>(new RigidBodyComponent(momo.get()));
    momo->addComponent(ComponentEnum::RIGID_BODY, std::move(rbc_2));

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
    up<MeshRendererComponent> mrc = up<MeshRendererComponent>(new MeshRendererComponent(go.get(), mObjRess[objName]->getMesh(go->name)));
    go->addComponent(ComponentEnum::MESH_RENDERER, std::move(mrc));
    
    up<DebugDrawComponent> ddc = up<DebugDrawComponent>(new DebugDrawComponent(go.get()));
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