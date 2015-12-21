#include "OMGame.h"
#include "Texture.h"
#include "Camera.h"
#include "Illuminator.hpp"
#include "Scene.hpp"
#include "PhysicalWorld.hpp"

#include "GameObject.hpp"
#include "Obj.h"

#include "OMUtils.h"

#include "WiredCube.hpp"

#include "Font.hpp"


#include <future>
#include <thread>
#include <chrono>
#include <iostream>

using UserData = struct{};
static OMContext       *appContext;
bool OMGame::debugFlag  = true;


// TODO: remove
Font *f;
std::future<bool> asyncFuture;

void drawText(string text){
    v4d fontColor(1.0f, .5f, 1.0f, 1.0f);
    f->print(-100, -100, text, &fontColor);
}
//-----------------

OMContext* OMGame::getAppContext(){
    if(!appContext) logMessage("\nAppContext is NULL!\n");
    return appContext;
}


int OMGame::InitGameWorld (){

    Scene::instance()->init();
    
// TODO: Temp
    f = new Font("foo.ttf", 64, 512, 512, 32, 96);
   
//    asyncFuture = std::async(std::launch::async, &Boombox::checkObbFunctionality, Boombox::instance());    
//---------

    return 1;
}


void OMGame::Update(const float deltaTime){
    Scene::instance()->update(deltaTime);
}


void OMGame::Draw (){
    Stopwatch stopwatch;
    
    Camera::instance()->setWidthAndHeight(appContext->width, appContext->height);

    
    
    glClearColor(0.0f, 0.3f, 0.0f, 1.0f);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
    
    Scene::instance()->draw();

//    Boombox::instance()->play();
    
    char tempBuff[122];
    sprintf(tempBuff, "%.0f", ceilf(stopwatch.fps()));
    
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

int OMGame::StartUp ( OMContext *context ){
    context->userData = malloc ( sizeof ( UserData ) );
    OMCreateWindow( context, "Hello Triangle", context->width, context->height, ES_WINDOW_RGB );
    appContext = context;
    appContext->shutdownFunc   = Shutdown;
    appContext->updateFunc     = Update;
    appContext->drawFunc       = Draw;
    appContext->touchFunc      = Touch;
    
    initOGL(context->width, context->height);
    
    atexit(Exit);
    
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
    printGLInfo();
    glViewport ( 0, 0, width, height );
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE  );
    glEnable(GL_TEXTURE_2D);
}



