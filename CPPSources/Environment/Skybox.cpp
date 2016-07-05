#include "Skybox.hpp"
#include "ShaderProgram.h"

static std::vector<float> cubeVerts = {
    -1.0f,  -1.0f,   1.0f,
    1.0f,  -1.0f,   1.0f,
    -1.0f,   1.0f,   1.0f,
    1.0f,   1.0f,   1.0f,
    -1.0f,  -1.0f,  -1.0f,
    1.0f,  -1.0f,  -1.0f,
    -1.0f,   1.0f,  -1.0f,
    1.0f,   1.0f,  -1.0f,
};

static std::vector<GLubyte> cubeIndices = {0, 1, 2, 3, 7, 1, 5, 4, 7, 6, 2, 4, 0, 1};

up<GameObject> Skybox::create(){
    up<GameObject> go = up<GameObject>(new GameObject("SKYBOX"));
    up<Skybox> skyboxComp = up<Skybox>(new Skybox(go.get()));
    go->addComponent(std::move(skyboxComp));
    return go;
}


Skybox::Skybox(GameObject * const gameObject) : IComponent(gameObject){
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glBufferData(GL_ARRAY_BUFFER, cubeVerts.size() * sizeof(float), &cubeVerts[0], GL_STATIC_DRAW );
    
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndices.size() * sizeof(GLubyte), &cubeIndices[0], GL_STATIC_DRAW);
    
    mComponentType = ComponentEnum::SKYBOX;
    Materials::instance()->loadTexture("Right.png", false);
    Materials::instance()->loadTexture("Left.png", false);
    Materials::instance()->loadTexture("Top.png", false);
    Materials::instance()->loadTexture("Bottom.png", false);
    Materials::instance()->loadTexture("Front.png", false);
    Materials::instance()->loadTexture("Back.png", false);
    
    glActiveTexture(GL_TEXTURE1);
    
    sp<Texture> right = Materials::instance()->getTexture("Right.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, right->width, right->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &right->texelArray[0] );
    
    sp<Texture> left = Materials::instance()->getTexture("Left.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, left->width, left->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &left->texelArray[0]);
    
    sp<Texture> top = Materials::instance()->getTexture("Top.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, top->width, top->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &top->texelArray[0]);
    
    sp<Texture> bottom = Materials::instance()->getTexture("Bottom.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, bottom->width, bottom->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &bottom->texelArray[0]);
    
    sp<Texture> front = Materials::instance()->getTexture("Front.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, front->width, front->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &front->texelArray[0]);
    
    sp<Texture> back = Materials::instance()->getTexture("Back.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, back->width, back->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &back->texelArray[0]);

    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

Skybox::~Skybox(){
}

bool Skybox::init(){

    
    
    logMessage("Init skybox goes here");
    return true;
}


void Skybox::draw(){
    sp<ShaderProgram> skyboxProgram = Materials::instance()->getProgram("skybox.omg");
}



