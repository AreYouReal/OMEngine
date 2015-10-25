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
    for(auto const &go : mObjects){
        go->draw();
    }
}

void Scene::setRenderObjectState(RenderObjectType newState){
    if(mDrawingState != newState) mDrawingState = newState;
}

// DEBUG AND TEST STUFF GOES HERE


void Scene::createTestScene(sp<Obj> object){
    /// TEST CODE
    v3d firstPos(0, 0, 0);
    
    
    up<GameObject> treeAndLeafs;
    up<GameObject> ground;
    up<GameObject> momo;
    
    
    treeAndLeafs = std::unique_ptr<GameObject>(new GameObject());
    treeAndLeafs->mTransform = std::make_shared<Transform>(v3d(0, 0, 5));
    treeAndLeafs->addObjMesh(object->getMesh("leaf"));
    treeAndLeafs->addObjMesh(object->getMesh("tree"));
    
    PhysicalWorld::instance()->addPBodyToGameObject(treeAndLeafs.get(), PhysicalBodyShape::BOX, 1.0f, treeAndLeafs->getDimensions());
    
    addObjOnScene(std::move(treeAndLeafs));
    
    momo = std::unique_ptr<GameObject>(new GameObject());
    momo->name = "momo";
    momo->mTransform = std::make_shared<Transform>(v3d(2.3, 0, 7));
    momo->addObjMesh(object->getMesh("momo"));
    
    PhysicalWorld::instance()->addPBodyToGameObject(momo.get(), PhysicalBodyShape::BOX, 1.0f, momo->getDimensions());
    
    addObjOnScene(std::move(momo));
    
    ground = std::unique_ptr<GameObject>(new GameObject());
    ground->mTransform = std::make_shared<Transform>(v3d(0, 0, 0));
    ground->addObjMesh(object->getMesh("grass_ground"));
    
    PhysicalWorld::instance()->addPBodyToGameObject(ground.get(), PhysicalBodyShape::BOX, 0.0f, ground->getDimensions());
    
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
    m->addObjMesh(mObjRess["scene"]->getMesh("momo"));
    
    PhysicalWorld::instance()->addPBodyToGameObject(m.get(), PhysicalBodyShape::BOX, 1.0f, m->getDimensions());
    
    addObjOnScene(std::move(m));
}