#include "Game.h"
#include "Texture.h"
#include "Camera.h"
#include "Illuminator.hpp"
#include "Scene.hpp"
#include "PhysicalWorld.hpp"

#include "GameObject.hpp"
#include "Obj.h"

// DEBUG AND TEST STUFF GOES HERE

sp<GameObject> treeAndLeafs;
sp<GameObject> ground;
sp<GameObject> momo;

void createTestScene(Scene *scene, sp<Obj> object){
    /// TEST CODE
    v3d firstPos(0, 0, 0);

    treeAndLeafs = std::make_shared<GameObject>();
    treeAndLeafs->mTransform = std::make_shared<Transform>(v3d(0, 0, 5));
    treeAndLeafs->addObjMesh(object->getMesh("leaf"));
    treeAndLeafs->addObjMesh(object->getMesh("tree"));
    scene->addObjOnScene(treeAndLeafs);
    
    
    momo = std::make_shared<GameObject>();
    momo->name = "momo";
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

sp<Camera>         cam;
sp<Illuminator>    ill;
sp<Materials>      mats;
sp<Scene>          scene;
sp<PhysicalWorld>  pWorld;


sp<Obj>    object;


void createMomo(Scene *scene, sp<Obj> object){
    sp<GameObject> m = std::make_shared<GameObject>();
    m->mTransform = std::make_shared<Transform>(v3d(2, 0, 10));
    m->addObjMesh(object->getMesh("momo"));
    scene->addObjOnScene(m);
    
    pWorld->addPBodyToGameObject(m, PhysicalBodyShape::BOX, 1.0f, m->getDimensions());
}

SRContext* Game::getAppContext(){
    if(!appContext) logMessage("\nAppContext is NULL!\n");
    return appContext;
}

///
// Initialize the shader and program object
//
int Game::Init ( SRContext *context ){
    printGLInfo();
    
    appContext = context;
    atexit(Exit);
    glViewport ( 0, 0, context->width, context->height );
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE  );
    glEnable(GL_TEXTURE_2D);

    mats        = up<Materials>( Materials::instance() );
    cam         = up<Camera>( Camera::instance() );
    ill         = up<Illuminator>( Illuminator::instance() );
    scene       = up<Scene>( Scene::instance() );
    pWorld      = up<PhysicalWorld>( PhysicalWorld::instance() );

    
    object = Obj::load("scene.obj");
    object->build();
    object->clear(); // Free mesh data.

    createTestScene(scene.get(), object);
    
    pWorld->addPBodyToGameObject(momo, PhysicalBodyShape::BOX, 1.0f, momo->getDimensions());
    pWorld->addPBodyToGameObject(treeAndLeafs, PhysicalBodyShape::BOX, 1.0f, treeAndLeafs->getDimensions());
    pWorld->addPBodyToGameObject(ground, PhysicalBodyShape::BOX, 0.0f, ground->getDimensions());
    
//    
//    , nullptr, [](btBroadphasePair &pair, btCollisionDispatcher &dispatcher, const btDispatcherInfo &info){
//        logMessage("nearCallback");
//        GameObject *go = (GameObject*)((btRigidBody*)(pair.m_pProxy0->m_clientObject))->getUserPointer();
//        if(go->name.compare("momo")) return;
//        
//        dispatcher.defaultNearCallback(pair, dispatcher, info);
//    }
    
    return true;
}


void Game::Update(SRContext *context, float deltaTime){
//    logMessage("UPDATE \n");
    pWorld->update(deltaTime);
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
    Camera::destroy();
    Illuminator::destroy();
    Scene::destroy();
    Materials::destroy();
    PhysicalWorld::destroy();
    cam = nullptr;
    ill = nullptr;
    scene = nullptr;
    mats = nullptr;
    pWorld = nullptr;
    
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
            createMomo(scene.get(), object);
            break;
        default:
            break;
    }
}

int Game::Main ( SRContext *context ){
    context->userData = malloc ( sizeof ( UserData ) );
    logMessage("Context value: %d\n", context);
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

#pragma Helpers
void Game::printGLInfo(){
    printGLString("Version",        GL_VERSION);
    printGLString("Vendor",         GL_VENDOR);
    printGLString("Renderer",       GL_RENDERER);
//    printGLString("Extensions",     GL_EXTENSIONS);
//    printGLString("GLSL version",   GL_SHADING_LANGUAGE_VERSION);
}



