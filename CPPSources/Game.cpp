#include "Game.h"
#include "ShaderLibrary.h"
#include "Texture.h"
#include "Camera.h"
#include "Illuminator.hpp"
#include "Scene.hpp"

#include "GameObject.hpp"
#include "Obj.h"


// DEBUG AND TEST STUFF GOES HERE

void createTestScene(sp<Scene> scene, sp<Obj> object){
    /// TEST CODE
    v3d firstPos(0, 0, 0);
    sp<Transform> rotateABit = sp<Transform>(new Transform());
    rotateABit->rotate(0, v3d(0.0f, 1.0f, 0.0f));
    
    sp<GameObject> firstObj = std::make_shared<GameObject>();
    firstObj->mTransform = std::make_shared<Transform>(firstPos);
    firstObj->addObjMesh(object->getMesh("leaf"));
    scene->addObjOnScene(firstObj);
    
    
    
    
//    up<GameObject> firstNode = up<ASceneNode>(new ASceneNode(sp<Transform>(new Transform(firstPos)), object->getMesh(0)));
//    up<ASceneNode> secondNode = up<ASceneNode>(new ASceneNode(sp<Transform>(new Transform()), object->getMesh(1)));
//    up<ASceneNode> thirdNode = up<ASceneNode>(new ASceneNode(rotateABit, object->getMesh(2)));
//    up<ASceneNode> onveMoreNode = up<ASceneNode>(new ASceneNode(sp<Transform>(new Transform()), object->getMesh(3)));
//    up<ASceneNode> moreNODE = up<ASceneNode>(new ASceneNode(sp<Transform>(new Transform()), object->getMesh(4)));
//    up<ASceneNode> leafts = up<ASceneNode>(new ASceneNode(sp<Transform>(new Transform()), object->getMesh(5)));
//    thirdNode->addChild(std::move(onveMoreNode));
//    firstNode->addChild(std::move(secondNode));
//    scene->addChild(std::move(thirdNode));
//    scene->addChild(std::move(firstNode));
//    scene->addChild(std::move(moreNODE));
//    scene->addChild(std::move(leafts));
    /// __________________________

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
    
    
    return true;
}


void Game::Update(SRContext *context, float deltaTime){
//    logMessage("UPDATE \n");
}


///
// Draw a triangle using the shader pair created in Init()
//
void Game::Draw ( SRContext *context ){
//    logMessage("DRAW \n");
#ifdef ANDROID
//    logMessage("%d, %d, %d, %d, %d\n", context->eglNativeDisplay, context->eglNativeWindow, context->eglDisplay, context->eglContext, context->eglSurface );
    if(!context->eglDisplay) return;
#endif
    
    Camera::instance()->setWidthAndHeight(context->width, context->height);

    glClearColor(0.0f, 0.3f, 0.0f, 1.0f);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );

        scene->update();
    
    // Draw all objects on the scene
//    scene->update();
}


void Game::Shutdown ( SRContext *context ){
    object.reset();
    sLibrary.reset();
    cam.reset();
    ill.reset();
    scene.reset();
    mats.reset();
    
    logMessage("obj: %d\n sl: %d\n cam: %d\n ill: %d\n scene: %d\n mats: %d\n", object.use_count(), sLibrary.use_count(), cam.use_count(), ill.use_count(), scene.use_count(), mats.use_count());
    
    
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






