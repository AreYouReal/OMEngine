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

Font *f;

void drawText(string text){
    v4d fontColor(1.0f, .5f, 1.0f, 1.0f);
    f->print(-100, -100, text, &fontColor);
}

OMContext* OMGame::getAppContext(){
    if(!appContext) logMessage("\nAppContext is NULL!\n");
    return appContext;
}

std::future<bool> asyncFuture;


int OMGame::InitGameWorld ( OMContext *context ){
    appContext = context;
    
    printGLInfo();
    initOGL(context->width, context->height);
    atexit(Exit);
    
    Scene::instance()->init();
    
    
    // Need to be refactored
    
    f = new Font("foo.ttf", 64, 512, 512, 32, 96);
    
    asyncFuture = std::async(std::launch::async, &Boombox::checkObbFunctionality, Boombox::instance());    
   //------------------------

    return true;
}


void OMGame::Update(OMContext *context, const float deltaTime){
//    logMessage("UPDATE \n");
    Scene::instance()->update(deltaTime);
}


void OMGame::Draw ( OMContext *context ){
    Stopwatch stopwatch;
    //    logMessage("DRAW \n");
#ifdef ANDROID
    if(!context->eglDisplay) return;
#endif
    
    Camera::instance()->setWidthAndHeight(context->width, context->height);

    glClearColor(0.0f, 0.3f, 0.0f, 1.0f);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );

    Scene::instance()->draw();
    

    
    Boombox::instance()->play();
    
    char tempBuff[122];
    sprintf(tempBuff, "%f", ceilf(stopwatch.fps()));
    
    drawText(tempBuff);
//    logMessage("%f\n", stopwatch.fps());
}


void OMGame::Shutdown ( OMContext *context ){
    Scene::destroy();
    if(f){
        delete f;
        f = 0;
    }
    logMessage("ShutDown function\n");
}

void OMGame::Touch(OMContext *context, const int event, const int x, const int y){
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

int OMGame::StartUp ( OMContext *context ){
    context->userData = malloc ( sizeof ( UserData ) );
    logMessage("Context value: %d\n", context);
    OMCreateWindow( context, "Hello Triangle", context->width, context->height, ES_WINDOW_RGB );

    if ( !OMGame::InitGameWorld( context ) ){ return GL_FALSE; }

    context->shutdownFunc = Shutdown;
    context->updateFunc = Update;
    context->drawFunc = Draw;
    context->touchFunc = Touch;

    return GL_TRUE;
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
    glViewport ( 0, 0, width, height );
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE  );
    glEnable(GL_TEXTURE_2D);
}



