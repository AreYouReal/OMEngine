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
    glBindAttribLocation(program->ID, 0, "vPosition");
    glBindAttribLocation(program->ID, 2, "vTexCoord");
    // BindLocations here
}

void materialDrawCallback(void *ptr){
    ObjMaterial *mat = (ObjMaterial *)ptr;
    ShaderProgram *program = mat->program;
    for(unsigned short i = 0; i < program->uniformCount; ++i){
        if(!strcmp(program->uniformArray[i].name.c_str(), "Diffuse")){
            glUniform1i(program->uniformArray[i].location, 1);
        }else if(!strcmp(program->uniformArray[i].name.c_str(), "modelViewM")){
            glUniformMatrix4fv(*program->uniformArray[i].name.c_str(), 1, GL_TRUE, ModelViewMatrix.pointer());
        }else if(!strcmp(program->uniformArray[i].name.c_str(), "projectionM")){
            glUniformMatrix4fv(*program->uniformArray[i].name.c_str(), 1, GL_TRUE, ProjectionMatrix.pointer());
        }
    }
}

void calculateMatrices(float width, float height){
    v3d eye(0.0f, -6.0f, 1.35f);
    v3d lookAt(0.0f, -5.0f, 1.35f);
    v3d up(0.0f, 0.0f, 1.0f);
    rotateObjMatrix = m4d::rotate(rotateAngel, 0, 0, 1); // * m4d::rotate(rotateAngel, 1, 0, 0);
    
    ModelViewMatrix     = m4d::lookAt(eye, lookAt, up) * rotateObjMatrix;
    ProjectionMatrix    = m4d::perspective(45, width, height, 0.1, 100);
    NormalMatrix        = m4d::inverseTranspose(ModelViewMatrix);
}




SRContext* SRGraphics::getAppContext(){
    return appContext;
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
    
    return true;
}

///
// Draw a triangle using the shader pair created in Init()
//
void SRGraphics::Draw ( SRContext *context ){

    calculateMatrices(context->width, context->height);

    glClearColor(0.0f, 0.3f, 0.0f, 1.0f);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );

    for(unsigned int i = 0; i < object->meshes.size(); ++i){
        object->drawMesh(i);
    }

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


