#include "main.h"
#include "ShaderHelper.h"
#include "SRUtils.h"

typedef struct{
   GLuint programObject;
} UserData;

///
// Initialize the shader and program object
//
int Init ( ESContext *esContext ){
   UserData *userData = (UserData*)esContext->userData;
    GLuint programObject;
    
    
//    Memory *m =  mopen("vertex.glsl", true);
//    Memory *m2 =  mopen("fragment.glsl", true);
//    programObject = ShaderHelper::createProgram((char*)m->buffer, (char*)m2->buffer);
    
    
    std::shared_ptr<ShaderSource> vertexSource =  readShaderFromFile(esContext, "vertex.glsl");
    std::shared_ptr<ShaderSource> fragmentSource =  readShaderFromFile(esContext, "fragment.glsl");
    logMessage("%d", fragmentSource->size);
    logMessage("%d", vertexSource->size);
    programObject = ShaderHelper::createProgram(vertexSource->source, fragmentSource->source);

    
    
//   const char* vS = "#version 300 es \n layout(location = 0) in vec4 vPosition;\n void main(){ gl_Position = vPosition; } \n";
//    
//    const char* fS = "#version 300 es \n precision mediump float;\n out vec4 fragColor; \n void main(){ fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 ); }";
//    
//    programObject = ShaderHelper::createProgram(vS, fS);
    
    
    
    
   if ( programObject == 0 ){
      return 0;
   }
    
   // Store the program object
   userData->programObject = programObject;
    
    
logMessage("program object %d", userData->programObject);

   glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
    
    return TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext ){
   UserData *userData = (UserData*)esContext->userData;
   GLfloat vVertices[] = {  0.0f,  0.5f, 0.0f,
                            -0.5f, -0.5f, 0.0f,
                            0.5f, -0.5f, 0.0f
                         };

   // Set the viewport
   glViewport ( 0, 0, esContext->width, esContext->height );

   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT );

   // Use the program object
   glUseProgram ( userData->programObject );

   // Load the vertex data
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
   glEnableVertexAttribArray ( 0 );

   glDrawArrays ( GL_TRIANGLES, 0, 3 );
}

void Shutdown ( ESContext *esContext ){
   UserData *userData = (UserData*)esContext->userData;

   glDeleteProgram ( userData->programObject );
}

int Main ( ESContext *esContext ){
   esContext->userData = malloc ( sizeof ( UserData ) );

   esCreateWindow ( esContext, "Hello Triangle", 320, 240, ES_WINDOW_RGB );

   if ( !Init ( esContext ) ){
      return GL_FALSE;
   }

   esRegisterShutdownFunc ( esContext, Shutdown );
   esRegisterDrawFunc ( esContext, Draw );

   return GL_TRUE;
}
