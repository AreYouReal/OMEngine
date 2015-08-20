#include "main.h"
#include "ShaderHelper.h"
#include "obj.h"

typedef struct{
    GLuint programObject;
    ShaderProgram* program;
} UserData;


static SRContext       *appContext;

m4d MVPMatrix;
v3d bve;

Obj     *object;
ObjMesh *objMesh;


SRContext* SRGraphics::getAppContext(){
    return appContext;
}

void programBindCallback(void *ptr){
    ShaderProgram *program = (ShaderProgram*)ptr;
    for(int i = 0; i < program->uniformCount; ++i){
        if(!strcmp( program->uniformArray[i].name, "MVPMatrix" )){
            glUniformMatrix4fv(program->uniformArray[i].location, 1, GL_TRUE, MVPMatrix.pointer());
        }
        
        
//        logMessage("UNiform: %s location %d", program->uniformArray[i].name, program->uniformArray[i].location);
    }
//    logMessage("programBindCallback in action!");
}


///
// Initialize the shader and program object
//
int SRGraphics::Init ( SRContext *context ){
    atexit(Exit);
    glViewport ( 0, 0, context->width, context->height );
    
    appContext = context;
    UserData *userData = (UserData*)context->userData;
    
    userData->program = ShaderHelper::createProgram("vertex.glsl", "fragment.glsl", programBindCallback, 0);
    
    object = Obj::load("model.obj");
    
    objMesh = &object->objMesh[0];
    
    unsigned char *vertexArray = NULL, *vertexStart = NULL;
    unsigned int  index = 0, size = 0;
    size = objMesh->nObjVertexData * sizeof(v3d) * sizeof(v3d);
    
    vertexArray = (unsigned char *)malloc(size);
    vertexStart = vertexArray;
    // OK->
    for(unsigned int i = 0; i < objMesh->nObjVertexData; ++i){
        index = objMesh->objVertexData[i].vIndex;
        memcpy(vertexArray, &object->vertices[index], sizeof(v3d));
        vertexArray += sizeof(v3d);
        memcpy(vertexArray, &object->normals[index], sizeof(v3d));
        vertexArray += sizeof(v3d);
    }
    
    for(int i = 0; i < size; ++i){
        logMessage("%u\t", vertexStart[i]);
    }

    glGenBuffers(1, &objMesh->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, objMesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, size, vertexStart, GL_STATIC_DRAW);
    if(vertexStart) delete vertexStart;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &objMesh->tList[0].vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objMesh->tList[0].vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, objMesh->tList[0].nIndices * sizeof(unsigned short), objMesh->tList[0].indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    // VAO
    unsigned char attribute, stride = sizeof(v3d) + sizeof(v3d);
    glGenVertexArrays(1, &objMesh->vao);
    glBindVertexArray(objMesh->vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, objMesh->vbo);
    attribute = userData->program->getVertexAttribLocation("vPosition");
    glEnableVertexAttribArray(attribute);
    glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_FALSE, stride, 0);
    
    attribute = userData->program->getVertexAttribLocation("vNormal");
    glEnableVertexAttribArray(attribute);
    glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_FALSE, stride, (char*) NULL + sizeof(v3d));
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objMesh->tList[0].vbo);
    glBindVertexArray(0);
    
    if ( userData->program == 0 ){ return 0; }

    return true;
}

///
// Draw a triangle using the shader pair created in Init()
//
void SRGraphics::Draw ( SRContext *context ){
    v3d eye(1.0f, -4.0f, 0.0);
    v3d lookAt(0.0f, 0.0f, 0.0f);
    v3d up(0.0f, 0.0f, 1.0f);
    MVPMatrix =  m4d::perspective(45, context->width, context->height, 0.1, 100) * m4d::lookAt(eye, lookAt, up);
    
    
    UserData *uData = (UserData*)context->userData;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
    glBindVertexArray(objMesh->vao);
    
    uData->program->use();
    
    glDrawElements(GL_TRIANGLES, objMesh->tList[0].nIndices, GL_UNSIGNED_SHORT, 0);
}

void SRGraphics::Shutdown ( SRContext *context ){
    if(object) delete object;
    logMessage("ShutDown function");
    UserData *userData = (UserData*)context->userData;

   glDeleteProgram ( userData->program->ID );
}

void SRGraphics::Touch(SRContext *context, int event, int x, int y){
//    logMessage("TOUCH OCCURED: %d, [ %d, %d ]", event, x, y);
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


