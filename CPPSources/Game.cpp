#include "Game.h"
#include "ShaderLibrary.h"
#include "Texture.h"
#include "Camera.h"
#include "Illuminator.hpp"
#include "Scene.hpp"

#include "GameObject.hpp"
#include "ObjMeshData.h"

#define CLAMP(x, min, max) ((x < min) ? min : ((x > max) ? max : x));

using UserData = struct{};

static SRContext       *appContext;

up<ShaderLibrary>  sLibrary;
up<Camera>         cam;
up<Illuminator>    ill;
up<Materials>      mats;
up<Scene>          scene;

sp<ObjMeshData>    object;
sp<GameObject>         gameObjects;


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

    mats        = up<Materials>(Materials::instance());
    sLibrary    = up<ShaderLibrary>(ShaderLibrary::instance());
    cam         = up<Camera>(Camera::instance());
    ill         = up<Illuminator>(Illuminator::instance());
    scene       = up<Scene>(Scene::instance());
    
    object = ObjMeshData::load("scene.obj");

    for(unsigned int i = 0; i < object->meshesSize(); ++i){
        ObjMesh *mesh = object->getMesh(i);
        mesh->build();
    }
    object->clear(); // Free mesh data.
    
    /// TEST CODE
    v3d firstPos(0, 0, 2);
    Transform *rotateABit = new Transform();
    rotateABit->rotate(90, v3d(0.0f, 1.0f, 0.0f));
    ASceneNode *firstNode = new ASceneNode(new Transform(firstPos), object->getMesh(0));
    ASceneNode *secondNode = new ASceneNode(new Transform(), object->getMesh(1));
    ASceneNode *thirdNode = new ASceneNode(rotateABit, object->getMesh(2));
    ASceneNode *onveMoreNode = new ASceneNode(new Transform(), object->getMesh(3));
    thirdNode->addChild(onveMoreNode);
    firstNode->addChild(secondNode);
    
    scene->addChild(thirdNode);
    
    scene->addChild(firstNode);
    /// __________________________
    
    return true;
}


void Game::Update(SRContext *context, float deltaTime){
    logMessage("FPS %f\n", 1.0f/deltaTime);
    logMessage("deltaTime: %f\n", deltaTime);
}


///
// Draw a triangle using the shader pair created in Init()
//
void Game::Draw ( SRContext *context ){
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
    logMessage("ShutDown function\n");
}


float touchX, touchY, deltaX, deltaY;
void Game::Touch(SRContext *context, int event, int x, int y){
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


