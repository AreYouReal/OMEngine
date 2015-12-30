#include "LightSource.hpp"

#include "OMUtils.h"

static v3d cubeVertices[] = {
    v3d(-.5f, -.5f, .5f),   // 0
    v3d(.5f, -.5f, .5f),    // 1
    v3d(.5f, .5f, .5f),     // 2
    v3d(-.5f, .5f, .5f),    // 3
    v3d(-.5f, .5f, -.5f),   // 4
    v3d(.5f, .5f, -.5f),    // 5
    v3d(.5f, -.5f, -.5f),   // 6
    v3d(-.5f, -.5f, -.5f)   // 7
};


LightSource::LightSource() : mPosition(-1.3f, -1.3f, 2.9f, 1.0f), mColor(1.0f, 1.0f, 1.0f, 1.0f){
    logMessage("LightSource constructor!\n");
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v3d) * 8, &cubeVertices[0], GL_STATIC_DRAW);
    mMaterial = Materials::instance()->getMaterial("lightPoint");
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    setAttributes();
    glBindVertexArray(0);
}

LightSource::LightSource(v4d position) : mPosition(position){ }

LightSource::~LightSource(){
    logMessage("Light Source destructor\n");
}

void LightSource::setAttributes() const{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    unsigned char size = sizeof(v3d) / sizeof(float);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, 0);
}

void LightSource::draw() const{
    if(vao) glBindVertexArray(vao); else setAttributes();
    mMaterial->use();
    glDrawArrays(GL_POINTS, 0, 8);
}