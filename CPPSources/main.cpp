#include "main.h"
#include "ShaderProgram.h"

typedef struct{
   GLuint programObject;
} UserData;

ShaderProgram *program;


static SRContext       *appContext;

SRContext* SRGraphics::GetAppContext(){
    return appContext;
}

///
// Initialize the shader and program object
//
int SRGraphics::Init ( SRContext *context ){
   UserData *userData = (UserData*)context->userData;
    GLuint programObject;
    appContext = context;
    
//    Memory *m =  mopen("vertex.glsl", true);
//    Memory *m2 =  mopen("fragment.glsl", true);
//    programObject = ShaderHelper::createProgram((char*)m->buffer, (char*)m2->buffer);
    
//    
//    std::shared_ptr<ShaderSource> vertexSource =  readShaderFromFile(context, "vertex.glsl");
//    std::shared_ptr<ShaderSource> fragmentSource =  readShaderFromFile(context, "fragment.glsl");
//    logMessage("%d", fragmentSource->size);
//    logMessage("%d", vertexSource->size);
    

//    readOBJFromFile(context, "model.obj");
    
     program = new ShaderProgram("PROGRAM", "vertex.glsl", "fragment.glsl", true, 0, 0);
    
//   const char* vS = "#version 300 es \n layout(location = 0) in vec4 vPosition;\n void main(){ gl_Position = vPosition; } \n";
//    
//    const char* fS = "#version 300 es \n precision mediump float;\n out vec4 fragColor; \n void main(){ fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 ); }";
    
//    programObject = ShaderHelper::createProgram(vertexSource->source, fragmentSource->source);
    
    
    
    
   if ( program == 0 ){
      return 0;
   }
    
   // Store the program object
   userData->programObject = program->ID;
    
    
logMessage("program object %d", userData->programObject);

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
   glUseProgram ( userData->programObject );

   // Load the vertex data
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
   glEnableVertexAttribArray ( 0 );

   glDrawArrays ( GL_TRIANGLES, 0, 3 );
}

void SRGraphics::Shutdown ( SRContext *context ){
   UserData *userData = (UserData*)context->userData;

   glDeleteProgram ( userData->programObject );
}

void SRGraphics::Touch(SRContext *context, int event, int x, int y){
    logMessage("TOUCH OCCURED: %d, [ %d, %d ]", event, x, y);
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


