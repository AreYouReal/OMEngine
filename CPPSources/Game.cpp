#include "Game.h"
#include "ShaderLibrary.h"
#include "Texture.h"
#include "Camera.h"
#include "Illuminator.hpp"
#include "Scene.hpp"

#include "GameObject.hpp"
#include "Obj.h"

// Buillet physics test code goes here
#include "btSoftBodyRigidBodyCollisionConfiguration.h"

#include "btDbvtBroadphase.h"
#include "btCollisionDispatcher.h"
#include "btBroadphaseInterface.h"
#include "btConstraintSolver.h"
#include "btSequentialImpulseConstraintSolver.h"
#include "btSoftRigidDynamicsWorld.h"

#include "btBoxShape.h"

#include "btDefaultMotionState.h"

sp<btSoftBodyRigidBodyCollisionConfiguration>   cConfig     = nullptr;
sp<btCollisionDispatcher>                       cDispatcher = nullptr;
sp<btBroadphaseInterface>                       cInterface  = nullptr;
sp<btConstraintSolver>                          cSolver     = nullptr;
sp<btSoftRigidDynamicsWorld>                    physicsWorld= nullptr;

void initPhysicsWorld(){
    cConfig      = std::make_shared<btSoftBodyRigidBodyCollisionConfiguration>();
    cDispatcher  = std::make_shared<btCollisionDispatcher>(cConfig.get());
    cInterface   = std::make_shared<btDbvtBroadphase>();
    cSolver      = std::make_shared<btSequentialImpulseConstraintSolver>();
    
    physicsWorld = std::make_shared<btSoftRigidDynamicsWorld>(cDispatcher.get(),
                                                              cInterface.get(),
                                                              cSolver.get(),
                                                              cConfig.get());
    physicsWorld->setGravity(btVector3(0, 0, -9.8f));
}


// Do I actualy need this when using smart-pointers? Think carefully...
void freePhysicsWorld(){
    while(physicsWorld->getNumCollisionObjects()){
        btCollisionObject *btCObject = physicsWorld->getCollisionObjectArray()[0];
        btRigidBody *rBody = btRigidBody::upcast(btCObject);
        if(rBody){
            delete rBody->getCollisionShape();
            delete rBody->getMotionState();
            physicsWorld->removeRigidBody(rBody);
            physicsWorld->removeCollisionObject(btCObject);
            delete rBody;
        }
    }
}

//------------------------------------


void addRigidBodyToTheGameObject(sp<GameObject> go, float mass){
    if(!go) return;
    v3d goDimensions = go->getDimensions();
    btCollisionShape *cShape = new btBoxShape(btVector3(goDimensions.x * 0.5f,
                                                                        goDimensions.y * 0.5f,
                                                                         goDimensions.z * 0.5f));
    btTransform bttransform;
    m4d transformM = m4d::transpose(go->mTransform->transformMatrix());
    bttransform.setFromOpenGLMatrix(transformM.pointer());
    btDefaultMotionState *defMState = new btDefaultMotionState(bttransform);
    btVector3 localInertia(0.0f, 0.0f, 0.0f);
    if(mass > 0.0f) cShape->calculateLocalInertia( mass, localInertia );
    go->pBody = new btRigidBody(mass, defMState, cShape, localInertia);
    go->pBody->setUserPointer(go.get());
    physicsWorld->addRigidBody(go->pBody);
}

// DEBUG AND TEST STUFF GOES HERE

sp<GameObject> treeAndLeafs;
sp<GameObject> ground;
sp<GameObject> momo;

void createTestScene(sp<Scene> scene, sp<Obj> object){
    /// TEST CODE
    v3d firstPos(0, 0, 0);

    treeAndLeafs = std::make_shared<GameObject>();
    treeAndLeafs->mTransform = std::make_shared<Transform>(v3d(0, 0, 5));
    treeAndLeafs->addObjMesh(object->getMesh("leaf"));
    treeAndLeafs->addObjMesh(object->getMesh("tree"));
    scene->addObjOnScene(treeAndLeafs);
    
    
    momo = std::make_shared<GameObject>();
    momo->mTransform = std::make_shared<Transform>(v3d(2.3, 0, 7));
    momo->addObjMesh(object->getMesh("momo"));
    scene->addObjOnScene(momo);
    
    ground = std::make_shared<GameObject>();
    ground->mTransform = std::make_shared<Transform>(v3d(0, 0, 0));
    ground->addObjMesh(object->getMesh("grass_ground"));
    scene->addObjOnScene(ground);
}

#define CLAMP(x, min, max) ((x < min) ? min : ((x > max) ? max : x));

using UserData = struct{};

static SRContext       *appContext;

sp<ShaderLibrary>  sLibrary;
sp<Camera>         cam;
sp<Illuminator>    ill;
sp<Materials>      mats;
sp<Scene>          scene;


sp<Obj>    object;


SRContext* Game::getAppContext(){
    if(!appContext) logMessage("\nAppContext is NULL!\n");
    return appContext;
}

///
// Initialize the shader and program object
//
int Game::Init ( SRContext *context ){
    appContext = context;
    atexit(Exit);
    glViewport ( 0, 0, context->width, context->height );
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE  );
    glEnable(GL_TEXTURE_2D);

    mats        = Materials::instance();
    sLibrary    = ShaderLibrary::instance();
    cam         = Camera::instance();
    ill         = Illuminator::instance();
    scene       = Scene::instance();

    
    object = Obj::load("scene.obj");
    object->build();
    object->clear(); // Free mesh data.

    createTestScene(scene, object);
    
    initPhysicsWorld();
    addRigidBodyToTheGameObject(treeAndLeafs, 1.0f );
    addRigidBodyToTheGameObject(ground, 0.0f);
    addRigidBodyToTheGameObject(momo, 1.0f);

    return true;
}


void Game::Update(SRContext *context, float deltaTime){
//    logMessage("UPDATE \n");
        physicsWorld->stepSimulation( 1.0f / 10.f );
}


///
// Draw a triangle using the shader pair created in Init()
//
void Game::Draw ( SRContext *context ){
//    Stopwatch stopwatch;
    //    logMessage("DRAW \n");
#ifdef ANDROID
//    logMessage("%d, %d, %d, %d, %d\n", context->eglNativeDisplay, context->eglNativeWindow, context->eglDisplay, context->eglContext, context->eglSurface );
    if(!context->eglDisplay) return;
#endif
    
    Camera::instance()->setWidthAndHeight(context->width, context->height);

    glClearColor(0.0f, 0.3f, 0.0f, 1.0f);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );

    scene->update();
//    logMessage("%f\n", stopwatch.fps());
}


void Game::Shutdown ( SRContext *context ){
    object.reset();
    sLibrary.reset();
    cam.reset();
    ill.reset();
    scene.reset();
    mats.reset();
    
    logMessage("obj: %d\n sl: %d\n cam: %d\n ill: %d\n scene: %d\n mats: %d\n", object.use_count(), sLibrary.use_count(), cam.use_count(), ill.use_count(), scene.use_count(), mats.use_count());
    
    freePhysicsWorld();
    
    logMessage("ShutDown function\n");
}


float touchX, touchY, deltaX, deltaY;
void Game::Touch(SRContext *context, int event, int x, int y){
//    logMessage("TOUCH\n");
    switch (event) {
        case TOUCH_EVENT::BEGAN  :
            touchX = x;
            touchY = y;
            break;
        case TOUCH_EVENT::MOVED  :
            deltaX = deltaX * 0.9f + 0.1f * CLAMP(touchX - x, -0.1f, 0.1f);
            deltaY = deltaY * 0.9f + 0.1f * CLAMP(touchY - y, -2.0f, 2.0f);
            touchX = x; touchY = y;
            Camera::instance()->rotate(deltaX * 10, 0.0f, 0.0f, 1.0f);
            Camera::instance()->move(deltaY * 0.1f);
            break;
        case TOUCH_EVENT::ENDED  :
        case TOUCH_EVENT::CANCELLED  :
            touchX = touchY = 0;
            break;
        default:
            break;
    }
}

int Game::Main ( SRContext *context ){
    context->userData = malloc ( sizeof ( UserData ) );

    SRCreateWindow( context, "Hello Triangle", context->width, context->height, ES_WINDOW_RGB );

    if ( !Game::Init ( context ) ){ return GL_FALSE; }

    SRRegisterShutdownFunc ( context, Shutdown );
    SRRegisterUpdateFunc(context, Update);
    SRRegisterDrawFunc ( context, Draw );
    SRRegisterTouchFunc( context, Touch );

    return GL_TRUE;
}

void Game::Exit(){
    logMessage("Exit function\n");
}






