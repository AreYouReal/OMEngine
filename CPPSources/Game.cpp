#include "Game.h"
#include "Texture.h"
#include "Camera.h"
#include "Illuminator.hpp"
#include "Scene.hpp"
#include "PhysicalWorld.hpp"

#include "GameObject.hpp"
#include "Obj.h"


#include "WiredCube.hpp"

#include "Font.hpp"

using UserData = struct{};

static SRContext       *appContext;

bool Game::debugFlag  = true;

Font *f;

SRContext* Game::getAppContext(){
    if(!appContext) logMessage("\nAppContext is NULL!\n");
    return appContext;
}

///
// Initialize the shader and program object
//
int Game::Init ( SRContext *context ){
    appContext = context;
    
    printGLInfo();
    initOGL(context->width, context->height);
    atexit(Exit);
    
    Scene::instance()->init();
    
    f = new Font("foo.ttf", 64, 512, 512, 32, 96);
    
    return true;
}


void Game::Update(SRContext *context, const float deltaTime){
//    logMessage("UPDATE \n");
    Scene::instance()->update(deltaTime);
}

///
// Draw a triangle using the shader pair created in Init()
//
void Game::Draw ( SRContext *context ){
//    Stopwatch stopwatch;
    //    logMessage("DRAW \n");
#ifdef ANDROID
    if(!context->eglDisplay) return;
#endif
    
    Camera::instance()->setWidthAndHeight(context->width, context->height);

    glClearColor(0.0f, 0.3f, 0.0f, 1.0f);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );

    Scene::instance()->draw();
    v4d fontColor(1.0f, .5f, 1.0f, 1.0f);
    f->print(-100, -100, "Test text", &fontColor);
//    logMessage("%f\n", stopwatch.fps());
}


void Game::Shutdown ( SRContext *context ){
    Scene::destroy();
    logMessage("ShutDown function\n");
}

void Game::Touch(SRContext *context, const int event, const int x, const int y){
//    logMessage("TOUCH\n");
    switch (event) {
        case static_cast<int>(TouchState::BEGIN):
            Camera::instance()->onTouchBegin(x, y);
            break;
        case static_cast<int>(TouchState::MOVED):
            Camera::instance()->onTouchMove(x, y);
            break;
        case static_cast<int>(TouchState::ENDED):
        case static_cast<int>(TouchState::CANCELLED):
            Camera::instance()->onTouchEnd(x, y);
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


void Game::initOGL(const float width, const float height){
    glViewport ( 0, 0, width, height );
    glEnable( GL_DEPTH_TEST );
    //    glEnable( GL_CULL_FACE  );
    glEnable(GL_TEXTURE_2D);
}



