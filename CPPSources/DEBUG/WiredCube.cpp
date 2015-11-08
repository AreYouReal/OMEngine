
#include "WiredCube.hpp"
#include "Materials.hpp"


std::vector<v3d>            WiredCube::vertices = {v3d(0, 0, 0), v3d(1, 1, 1)};

WiredCube::WiredCube(){
    buildVBO();
    initMaterial();
    glGenVertexArrays(1, &glInfo.vao);
    glBindVertexArray(glInfo.vao);
    setAttributes();
    glBindVertexArray(0);
}

void WiredCube::buildVBO(){
    glInfo.stride = 0;
    
    glInfo.size = vertices.size() * sizeof(v3d);
    glGenBuffers(1, &glInfo.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, glInfo.vbo);
    glBufferData(GL_ARRAY_BUFFER, GL_FLOAT, &vertices[0], GL_STATIC_DRAW);
}

void WiredCube::initMaterial(){
    material = Materials::instance()->getMaterial("wired");
}

void WiredCube::setAttributes(){
    glBindBuffer(GL_ARRAY_BUFFER, glInfo.vbo);
    unsigned char size = sizeof(v3d) / sizeof(float);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, glInfo.stride, 0);
}

void WiredCube::draw(){
    if(glInfo.vao) glBindVertexArray(glInfo.vao); else setAttributes();
    glDrawArrays(GL_LINE_STRIP, 0, vertices.size());
}





