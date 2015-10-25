#include "Scene.hpp"

Scene::Scene(){
    
    Camera::instance();
    Illuminator::instance();
    Materials::instance();
    PhysicalWorld::instance();
    
    sp<Obj> object = Obj::load("scene.obj");
    object->build();
    object->clear(); // Free mesh data.

    mObjRess.insert(std::pair<string, sp<Obj>>("scene", object));
    
    createTestScene(object);
    
    logMessage("Scene constructor!\n");
}

Scene::~Scene(){
    mObjects.clear();

    Camera::destroy();
    Illuminator::destroy();
    Materials::destroy();
    PhysicalWorld::destroy();
    
    logMessage("Scene destructor!\n");
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
        if(mrc){ mrc->update(); }
    }
}

void Scene::setRenderObjectState(RenderObjectType newState){
    if(mDrawingState != newState) mDrawingState = newState;
}

// DEBUG AND TEST STUFF GOES HERE


void Scene::createTestScene(sp<Obj> object){
    /// TEST CODE
    v3d firstPos(0, 0, 0);
    
    up<GameObject> ground;
    
    
    // Tree
    
    up<GameObject> treeAndLeafs = std::unique_ptr<GameObject>(new GameObject());
    
    up<MeshRendererComponent> mrc_1 = up<MeshRendererComponent>(new MeshRendererComponent(treeAndLeafs.get()));
    mrc_1->addMesh(object->getMesh("leaf"));
    mrc_1->addMesh(object->getMesh("tree"));
    
    treeAndLeafs->mTransform = std::make_shared<Transform>(v3d(0, 0, 5));
    treeAndLeafs->addComponent(ComponentEnum::MESH_RENDERER, std::move(mrc_1));
    
    PhysicalWorld::instance()->addPBodyToGameObject(treeAndLeafs.get(), PhysicalBodyShape::BOX, 1.0f, treeAndLeafs->getDimensions());
    
    //------------------------------
    addObjOnScene(std::move(treeAndLeafs));
    
    // MOMO
    up<GameObject> momo = std::unique_ptr<GameObject>(new GameObject());

    mrc_1 = up<MeshRendererComponent>(new MeshRendererComponent(momo.get()));
    mrc_1->addMesh(object->getMesh("momo"));
    
    momo->name = "momo";
    momo->mTransform = std::make_shared<Transform>(v3d(2.3, 0, 7));
    momo->addComponent(ComponentEnum::MESH_RENDERER, std::move(mrc_1));
    
    
    PhysicalWorld::instance()->addPBodyToGameObject(momo.get(), PhysicalBodyShape::BOX, 1.0f, momo->getDimensions());
    
    //----------------------------
    addObjOnScene(std::move(momo));
    
    // GROUND
    
    ground = std::unique_ptr<GameObject>(new GameObject());
    ground->mTransform = std::make_shared<Transform>(v3d(0, 0, 0));

    
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
    m->mTransform = std::make_shared<Transform>(v3d(2, 0, 10));
    
    up<MeshRendererComponent> mrc_1 = up<MeshRendererComponent>(new MeshRendererComponent(m.get()));
    mrc_1->addMesh(mObjRess["scene"]->getMesh("momo"));
    
    m->addComponent(ComponentEnum::MESH_RENDERER, std::move(mrc_1));
    
    PhysicalWorld::instance()->addPBodyToGameObject(m.get(), PhysicalBodyShape::BOX, 1.0f, m->getDimensions());
    
    addObjOnScene(std::move(m));
}