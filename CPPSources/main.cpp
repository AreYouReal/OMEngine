#include "main.h"
#include "ShaderHelper.h"
#include "obj.h"
#include "Texture.h"

typedef struct{
    GLuint programObject;
    ShaderProgram* program;
} UserData;


static SRContext       *appContext;

m4d ModelViewMatrix;
m4d ProjectionMatrix;
m4d NormalMatrix;
v3d lightPosition(0.0f, 0.0f, 0.0f);

v3d bve;

Obj     *object;

int rotateAngel = 0;
m4d rotateObjMatrix;

void programBindAttributes(void *ptr){
    ShaderProgram *program = (ShaderProgram*) ptr;
    // BindLocations here
}

void materialDrawCallback(void *ptr){
    ObjMaterial *mat = (ObjMaterial *)ptr;
}

void calculateMatrices(float width, float height){
    v3d eye(0.0f, -4.0f, 0.0);
    v3d lookAt(0.0f, 0.0f, 0.0f);
    v3d up(0.0f, 0.0f, 1.0f);
    rotateObjMatrix = m4d::rotate(rotateAngel, 0, 0, 1);// * m4d::rotate(rotateAngel, 1, 0, 0);
    
    ModelViewMatrix     = m4d::lookAt(eye, lookAt, up) * rotateObjMatrix;
    ProjectionMatrix    = m4d::perspective(45, width, height, 0.1, 100);
    NormalMatrix        = m4d::inverseTranspose(ModelViewMatrix);
}




SRContext* SRGraphics::getAppContext(){
    return appContext;
}

void programBindCallback(void *ptr){
    ShaderProgram *program = (ShaderProgram*)ptr;
    for(int i = 0; i < program->uniformCount; ++i){
        if(!strcmp( program->uniformArray[i].name.c_str(), "modelViewM" )){
            glUniformMatrix4fv(program->uniformArray[i].location, 1, GL_TRUE, (float*)ModelViewMatrix.pointer());
        }else if(!strcmp( program->uniformArray[i].name.c_str(), "projectionM" )){
            glUniformMatrix4fv(program->uniformArray[i].location, 1, GL_TRUE, (float*)ProjectionMatrix.pointer());
        }else if(!strcmp( program->uniformArray[i].name.c_str(), "normalM" )){
            glUniformMatrix4fv(program->uniformArray[i].location, 1, GL_TRUE, (float*)NormalMatrix.pointer());
        }else if(!strcmp( program->uniformArray[i].name.c_str(), "lightPos" )){
            glUniform3fv(program->uniformArray[i].location, 1, &lightPosition.x);
        }else if(!strcmp( program->uniformArray[i].name.c_str(), "Diffuse" ) && !program->uniformArray[i].constant){
            glUniform1i(program->uniformArray[i].location, 0);
            program->uniformArray[i].constant = true;
        }
    }
}


///
// Initialize the shader and program object
//
int SRGraphics::Init ( SRContext *context ){
    
    atexit(Exit);
    glViewport ( 0, 0, context->width, context->height );
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE  );
    glEnable(GL_TEXTURE_2D);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_ONE, GL_SRC_COLOR);
    
    appContext = context;
    UserData *userData = (UserData*)context->userData;
    
    userData->program = ShaderHelper::createProgram("vertex.glsl", "fragment.glsl", programBindCallback, 0);
    
    object = new Obj("scene.obj");
    
    for(unsigned int i = 0; i < object->meshes.size(); ++i){
        object->buildMesh(i);
        // Free object mesh data if needed here
    }
    
    for(unsigned int i = 0; i < object->textures.size(); ++i){
        object->textures[i].generateID(TEXTURE_MIPMAP, TEXTURE_FILTER_2X);
    }
    
    for(unsigned int i = 0; i < object->materials.size(); ++i){
        object->buildMaterial(i, NULL);
        object->materials[i].program = ShaderHelper::createProgram("vertex.glsl", "fragment.glsl", programBindAttributes, NULL);
        object->materials[i].materialDrawCalback = materialDrawCallback;
    }  
    
//    objMesh = &object->meshes[0];
//
//    unsigned char *vertexArray = NULL, *vertexStart = NULL;
//    unsigned int  index = 0, size = 0;
//    size = objMesh->vertexData.size() * sizeof(v3d) * sizeof(v3d) * sizeof(v3d);
//    
//    vertexArray = (unsigned char *)malloc(size);
//    vertexStart = vertexArray;
//    // OK->
//    for(unsigned int i = 0; i < objMesh->vertexData.size(); ++i){
//        index = objMesh->vertexData[i].vIndex;
//        memcpy(vertexArray, &object->vertices[index], sizeof(v3d));
//        vertexArray += sizeof(v3d);
//        memcpy(vertexArray, &object->normals[index], sizeof(v3d));
//        vertexArray += sizeof(v3d);
//        memcpy(vertexArray, &object->UVs[objMesh->vertexData[i].uvIndex], sizeof(v3d));
//        vertexArray += sizeof(v3d);
//    }
//
//    glGenBuffers(1, &objMesh->vbo);
//    glBindBuffer(GL_ARRAY_BUFFER, objMesh->vbo);
//    glBufferData(GL_ARRAY_BUFFER, size, vertexStart, GL_STATIC_DRAW);
//    if(vertexStart) delete vertexStart;
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    
//    glGenBuffers(1, &objMesh->tLists[0].vbo);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objMesh->tLists[0].vbo);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, objMesh->tLists[0].indices.size() * sizeof(unsigned short), &objMesh->tLists[0].indices[0], GL_STATIC_DRAW);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//    
//    // VAO
//    unsigned char attribute, stride = sizeof(v3d) + sizeof(v3d) + sizeof(v3d);
//    glGenVertexArrays(1, &objMesh->vao);
//    glBindVertexArray(objMesh->vao);
//    
//    glBindBuffer(GL_ARRAY_BUFFER, objMesh->vbo);
//    attribute = userData->program->getVertexAttribLocation("vPosition");
//    glEnableVertexAttribArray(attribute);
//    glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_FALSE, stride, 0);
//    
//    attribute = userData->program->getVertexAttribLocation("vNormal");
//    glEnableVertexAttribArray(attribute);
//    glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_FALSE, stride, (char*) NULL + sizeof(v3d));
//    
//    attribute = userData->program->getVertexAttribLocation("vTexCoord");
//    glEnableVertexAttribArray(attribute);
//    glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_FALSE, stride, (char*)NULL + sizeof(v3d) + sizeof(v3d));
//    
//    
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objMesh->tLists[0].vbo);
//    glBindVertexArray(0);
//    
//    if ( userData->program == 0 ){ return 0; }
    
    return true;
}

///
// Draw a triangle using the shader pair created in Init()
//
void SRGraphics::Draw ( SRContext *context ){
//
//    calculateMatrices(context->width, context->height);
//    
//    UserData *uData = (UserData*)context->userData;
//    
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, texture->ID);
//    
//    
    glClearColor(0.0f, 0.3f, 0.0f, 1.0f);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
//    glBindVertexArray(objMesh->vao);
//    
//
//    uData->program->use();
//    
//
//    
//    if(object) glDrawElements(GL_TRIANGLES, object->meshes[0].tLists[0].indices.size(), GL_UNSIGNED_SHORT, 0);
}


void SRGraphics::Shutdown ( SRContext *context ){
    if(object) delete object;
    logMessage("ShutDown function\n");
    UserData *userData = (UserData*)context->userData;

   glDeleteProgram ( userData->program->ID );
}

int startX, startY;
void SRGraphics::Touch(SRContext *context, int event, int x, int y){
    switch (event) {
        case TOUCH_EVENT::BEGAN  :
            startX = x;
            startY = y;
            break;
        case TOUCH_EVENT::MOVED  :
            rotateAngel -= ((startX - x) + (startY - y))/20;
            rotateAngel %= 360;
            break;
        case TOUCH_EVENT::ENDED  :
        case TOUCH_EVENT::CANCELLED  :
            startX = startY = 0;
            break;
        default:
            break;
    }
    
    
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
    logMessage("Exit function\n");
}


