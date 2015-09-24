#include "Game.h"
#include "ShaderLibrary.h"
#include "obj.h"
#include "Texture.h"
#include "Camera.h"
#include "Illuminator.hpp"

#include "GameObject.hpp"

typedef struct{ } UserData;

static SRContext       *appContext;

std::shared_ptr<Obj>     object;

std::shared_ptr<GameObject> gameObject(new GameObject());

SRContext* Game::getAppContext(){ return appContext; }

///
// Initialize the shader and program object
//
int Game::Init ( SRContext *context ){
    atexit(Exit);
    glViewport ( 0, 0, context->width, context->height );
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE  );
    glEnable(GL_TEXTURE_2D);
    
    appContext = context;
    
    object = std::shared_ptr<Obj>(new Obj("samurai_monastry.obj"));
    
    ShaderLibrary::instance();
    Camera::createCamera();
    Illuminator::instance();
    
    
    // TODO: UNITE THREE LOOPS
    for(unsigned int i = 0; i < object->meshesSize(); ++i){
//        object->optimizeMesh(i, 0);
        object->getMesh(i)->build();
        // Free object mesh data if needed here
    }
    
    for(unsigned int i = 0; i < object->texturesSize(); ++i){
        object->generateTextureID(i, 0, 0);
//        object->textures[i].generateID(TEXTURE_MIPMAP, TEXTURE_FILTER_2X);  // !!!!!!
    }
    
    for(unsigned int i = 0; i < object->materialsSize(); ++i){
        object->buildMaterial(i, NULL);
        object->SetMaterialProgram(i);
    }
    
    gameObject->addObjMesh(object->getMesh(0));
    
    return true;
}

///
// Draw a triangle using the shader pair created in Init()
//
void Game::Draw ( SRContext *context ){
//    Stopwatch drawTimer;
    
#ifdef ANDROID
//    logMessage("%d, %d, %d, %d, %d\n", context->eglNativeDisplay, context->eglNativeWindow, context->eglDisplay, context->eglContext, context->eglSurface );
    if(!context->eglDisplay) return;
#endif
    
    Camera::instance()->setWidthAndHeight(context->width, context->height);

    glClearColor(0.0f, 0.3f, 0.0f, 1.0f);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );

//    gameObject->draw();
    
    // Solid objects goes here
    for(unsigned int i = 0; i < object->meshesSize(); ++i){
        if(object->getMesh(i)->renderObjectType() == SOLID) object->drawMesh(i);
    }
//
////    for(unsigned int i = 0; i < object->meshesSize(); ++i){
////        if(object->renderObjectType(i) == ALPHA_TESTED){
////            glCullFace(GL_FRONT);
////            object->drawMesh(i);
////            glCullFace(GL_BACK);
////            object->drawMesh(i);
////        }
////    }
//    
    for(unsigned int i = 0; i < object->meshesSize(); ++i){
        if(object->getMesh(i)->renderObjectType() != SOLID){
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glCullFace(GL_FRONT);
            object->drawMesh(i);
            glCullFace(GL_BACK);
            object->drawMesh(i);
            glDisable(GL_BLEND);
        }
    }
//    logMessage("FPS: %f", drawTimer.fps());
}


void Game::Shutdown ( SRContext *context ){
//    logMessage("ShutDown function\n");
}

int startX, startY;
void Game::Touch(SRContext *context, int event, int x, int y){
    switch (event) {
        case TOUCH_EVENT::BEGAN  :
            startX = x;
            startY = y;
            break;
        case TOUCH_EVENT::MOVED  :{
            float deltaAngle =((startX - x) + (startY - y))/20;
            Camera::instance()->move(deltaAngle > 0);
        }
            break;
        case TOUCH_EVENT::ENDED  :
        case TOUCH_EVENT::CANCELLED  :
            startX = startY = 0;
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


