#include "OMGame.h"
#include "Texture.h"
#include "Camera.h"
#include "Scene.hpp"


#include "OMUtils.h"

#include "Font.hpp"

#include <future>
#include <thread>
#include <chrono>
#include <iostream>


using UserData = struct{};
static OMContext       *appContext;
bool OMGame::debugFlag  = true;

bool disableScene = false;

// TODO: remove
Font *f;
std::future<bool> asyncFuture;

void drawText(string text){
    v4d fontColor(.0f, 1.0f, 1.0f, 1.0f);
    f->print(-appContext->width/2, appContext->height/2 - f->size()*0.75, text, &fontColor);
}
//-----------------

OMContext* OMGame::getAppContext(){
    if(!appContext) logMessage("\nAppContext is NULL!\n");
    return appContext;
}

int OMGame::InitGameWorld (){
      if(disableScene) return 1;
    
    logGLError();

    Scene::instance()->init();
    
// TODO: Temp
    f = new Font("foo.ttf", 32, 512, 512, 32, 96);
   
//    asyncFuture = std::async(std::launch::async, &Boombox::checkObbFunctionality, Boombox::instance());    
//---------

    logGLError();
    
    return 1;
}


void OMGame::Update(const float deltaTime){
    if(disableScene) return;
    Time::deltaTime = deltaTime;
    Scene::instance()->update(deltaTime);
}

Stopwatch stopwatch;
char tempBuff[122];
void OMGame::Draw (){
    if(disableScene) return;
    static unsigned int start = getMilliTime(), fps = 0;
    if( getMilliTime() - start >= 1000 ) {

        sprintf(tempBuff, "%.0f", ceilf(fps));
//        logMessage( "FPS: %d\n", fps );
        start = getMilliTime();
        fps = 0;
    } ++fps;
    
    Camera::instance()->setWidthAndHeight(appContext->width, appContext->height);

    logGLError();
    
    Scene::instance()->drawDepth();

    Scene::instance()->draw();

//    Boombox::instance()->play();
    
    drawText(tempBuff);
}


void OMGame::Shutdown ( ){
    Scene::destroy();
    
    // TODO:
    if(f){
        delete f;
        f = 0;
    }
    //--------------
    
    logMessage("ShutDown function\n");
}

void OMGame::Touch(const int event, const int x, const int y){
        logGLError();
    switch (event) {
        case static_cast<int>(TouchState::BEGIN):
//            Camera::instance()->onTouchBegin(x, y);
            Scene::instance()->onTouchBegin(x, y);
            break;
        case static_cast<int>(TouchState::MOVED):
            Scene::instance()->onTouchMove(x, y);
//            Camera::instance()->onTouchMove(x, y);
            break;
        case static_cast<int>(TouchState::ENDED):
        case static_cast<int>(TouchState::CANCELLED):
            Scene::instance()->onTouchEnd(x, y);
//            Camera::instance()->onTouchEnd(x, y);
            break;
        default:
            break;
    }
}

int OMGame::StartUp ( OMContext *context ){
    logGLError();
    context->userData = malloc ( sizeof ( UserData ) );
    OMCreateWindow( context, "Hello Triangle", context->width, context->height, OM_WINDOW_RGB | OM_WINDOW_DEPTH );
    appContext = context;
    appContext->shutdownFunc   = Shutdown;
    appContext->updateFunc     = Update;
    appContext->drawFunc       = Draw;
    appContext->touchFunc      = Touch;
    
    initOGL(context->width, context->height);
    
    atexit(Exit);
    
        logGLError();
    return OMGame::InitGameWorld();
}

void OMGame::Exit(){
    logMessage("Exit function\n");
}

#pragma Helpers
void OMGame::printGLInfo(){
    printGLString("Version",        GL_VERSION);
    printGLString("Vendor",         GL_VENDOR);
    printGLString("Renderer",       GL_RENDERER);
//    printGLString("Extensions",     GL_EXTENSIONS);
//    printGLString("GLSL version",   GL_SHADING_LANGUAGE_VERSION);
}


void OMGame::initOGL(const float width, const float height){
    logGLError();
    printGLInfo();
    glViewport ( 0, 0, width, height );
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE  );
    logGLError();
}



