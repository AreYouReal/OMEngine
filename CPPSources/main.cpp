#include "main.h"
#include "ShaderHelper.h"
#include "obj.h"

typedef struct{
    GLuint programObject;
    ShaderProgram* program;
} UserData;

ShaderProgram* program;

static SRContext       *appContext;

SRContext* SRGraphics::getAppContext(){
    return appContext;
}

void programBindCallback(void *ptr){
    logMessage("programBindCallback in action!");
}


///
// Initialize the shader and program object
//
int SRGraphics::Init ( SRContext *context ){
    atexit(Exit);
   UserData *userData = (UserData*)context->userData;
    appContext = context;

    userData->program = ShaderHelper::createProgram("vertex.glsl", "fragment.glsl", programBindCallback, 0);
    
    Obj* obj = Obj::load("model.obj");
    
    if(obj) delete obj;
    
   if ( userData->program == 0 ){
      return 0;
   }
    
logMessage("program object %d", userData->program->ID);

   glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );

    return true;
}

///
// Draw a triangle using the shader pair created in Init()
//
void SRGraphics::Draw ( SRContext *context ){
   UserData *userData = (UserData*)context->userData;
   GLfloat vVertices[] = {  0.0f,  0.5f, 0.0f,
                            -0.5f, -0.5f, 0.0f,
                            0.5f, -0.5f, 0.0f
                         };

   // Set the viewport
   glViewport ( 0, 0, context->width, context->height );

   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT );

   // Use the program object
   glUseProgram ( userData->program->ID );
   // Load the vertex data
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
   glEnableVertexAttribArray ( 0 );

   glDrawArrays ( GL_TRIANGLES, 0, 3 );
    glUseProgram(0);
}

void SRGraphics::Shutdown ( SRContext *context ){
    logMessage("ShutDown function");
    UserData *userData = (UserData*)context->userData;

   glDeleteProgram ( userData->program->ID );
}

void SRGraphics::Touch(SRContext *context, int event, int x, int y){
    logMessage("TOUCH OCCURED: %d, [ %d, %d ]", event, x, y);
//    program.reset();
}

int SRGraphics::Main ( SRContext *context ){
    context->userData = malloc ( sizeof ( UserData ) );

    SRCreateWindow( context, "Hello Triangle", 320, 240, ES_WINDOW_RGB );

    if ( !SRGraphics::Init ( context ) ){ return GL_FALSE; }

    SRRegisterShutdownFunc ( context, Shutdown );
    SRRegisterDrawFunc ( context, Draw );
    SRRegisterTouchFunc( context, Touch );

    return GL_TRUE;
}

void SRGraphics::Exit(){
    logMessage("Exit function");
}


