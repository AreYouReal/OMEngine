#include "Game.h"
#include "ShaderLibrary.h"
#include "Texture.h"
#include "Camera.h"
#include "Illuminator.hpp"
#include "Scene.hpp"

#include "GameObject.hpp"
#include "ObjMeshData.h"

#define CLAMP(x, min, max) ((x < min) ? min : ((x > max) ? max : x));

typedef struct{ } UserData;

static SRContext       *appContext;

OBJMESHDATA    object;
std::shared_ptr<ShaderLibrary>  sLibrary;
std::shared_ptr<Camera>         cam;
std::shared_ptr<Illuminator>    ill;
std::shared_ptr<Materials>      mats;

std::unique_ptr<Scene>          scene;

std::vector<GameObject>         gameObjects;


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

    mats        = std::shared_ptr<Materials>(Materials::instance());
    sLibrary    = std::shared_ptr<ShaderLibrary>(ShaderLibrary::instance());
    cam         = std::shared_ptr<Camera>(Camera::instance());
    ill         = std::shared_ptr<Illuminator>(Illuminator::instance());
    scene       = std::unique_ptr<Scene>(Scene::instance());
    
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

///
// Draw a triangle using the shader pair created in Init()
//
void Game::Draw ( SRContext *context ){
    Stopwatch drawTimer;
    static float elapsedTime;
#ifdef ANDROID
//    logMessage("%d, %d, %d, %d, %d\n", context->eglNativeDisplay, context->eglNativeWindow, context->eglDisplay, context->eglContext, context->eglSurface );
    if(!context->eglDisplay) return;
#endif
    
    Camera::instance()->setWidthAndHeight(context->width, context->height);

    glClearColor(0.0f, 0.3f, 0.0f, 1.0f);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
    
    // Draw all objects on the scene
    scene->update();    

    elapsedTime+= drawTimer.elapsedTime();
    static int fps = 0;
    fps++;
    if(elapsedTime >= 1.0f){
        logMessage("FPS: %d\n",fps );
        fps = 0;
        elapsedTime = 0.0f;
    }
}


void Game::Shutdown ( SRContext *context ){
    object.reset();
    sLibrary.reset();
    cam.reset();
    ill.reset();
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

    SRCreateWindow( context, "Hello Triangle", 320, 240, ES_WINDOW_RGB );

    if ( !Game::Init ( context ) ){ return GL_FALSE; }

    SRRegisterShutdownFunc ( context, Shutdown );
    SRRegisterDrawFunc ( context, Draw );
    SRRegisterTouchFunc( context, Touch );

    return GL_TRUE;
}

void Game::Exit(){
    logMessage("Exit function\n");
}


