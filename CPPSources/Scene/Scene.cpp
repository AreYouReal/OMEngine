#include "Scene.hpp"

#include "Camera.h"

#include "Shortcuts.h"

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
    
//        createTestScene();
    
    createLightTestScene();
    logGLError();
    sp<Texture> projTexture = Materials::instance()->getTexture("projector.png");
    glBindTexture(GL_TEXTURE_2D, projTexture->ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    logGLError();
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
    
    Illuminator::instance()->update(deltaTime);
    Camera::instance()->refreshProjectorMatrix();
}

void Scene::drawDepth(){
    
//    unsigned int framebuffer;
//    unsigned int depthRenderbuffer;
//    unsigned int texture;
//    int         texWidth = 256, texHeight = 256;
//    int maxRenderbufferSize;
//    
//    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize);
//    if(maxRenderbufferSize <= texWidth || maxRenderbufferSize <= texHeight){
//        logMessage("Max renderbuffer size: %d", maxRenderbufferSize );
//        logMessage("Too big texture widht ot height!");
//    }
//    glGenFramebuffers   (1, &framebuffer);
//    glGenRenderbuffers  (1, &depthRenderbuffer);
//    glGenTextures       (1, &texture);
//    
//    glBindTexture(GL_TEXTURE_2D, texture);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL );
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    
//    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, texWidth, texHeight);
//    
//    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
//    
//    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//    if(status == GL_FRAMEBUFFER_COMPLETE  ){
//        logMessage("FRAMEBUFFER COMPLETE!!!");
//    }else{
//    
//    }
    
//    glBindFramebuffer( GL_FRAMEBUFFER, 1);
//    glViewport(0, 0, Camera::instance()->shadowmapWidth(), Camera::instance()->shadowmapHeight());
//    glClear(GL_DEPTH_BUFFER_BIT);
//    glCullFace(GL_FRONT);
    
//    for(const auto& go: mObjects){
//        MeshRendererComponent *mrc = static_cast<MeshRendererComponent*>(go->getComponent(ComponentEnum::MESH_RENDERER));
//        if(mrc){
//            mrc->shadowDraw = false;
//            mrc->draw();
//            mrc->shadowDraw = false;
//        }
//    }
//    glCullFace( GL_BACK );
}

void Scene::draw(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    logGLError();
    
//    unsigned int fbStatus = glCheckFramebufferStatus(0);
//    
//        logMessage("STATUS::::: %x\n", fbStatus);
//    glViewport(0, 0, Camera::instance()->width(), Camera::instance()->height());
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
    
    for(const auto& go : mObjects){
        for(int i = (int)ComponentEnum::MESH_RENDERER; i <= (int)ComponentEnum::DEBUG_DRAW; ++i){
            IComponent *comp = go->getComponent((ComponentEnum)i);
            if(comp){
                sp<Texture> projTexture = Materials::instance()->getTexture("projector.png");
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, projTexture->ID);
                comp->draw();
            }
        }
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
    up<MeshRendererComponent> mrc = up<MeshRendererComponent>(new MeshRendererComponent(go.get(), mObjRess[objName]->getMesh(go->name)));
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
//        if(!mesh->getName().compare("projector")) continue;
        addMeshRendererOnScene("lightScene", mesh->getName());
    }
}