
#include "WiredCube.hpp"
#include "Materials.hpp"

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

std::vector<v3d>            WiredCube::vertices{
    cubeVertices[0],cubeVertices[1],cubeVertices[2],cubeVertices[3],cubeVertices[4],cubeVertices[5],cubeVertices[6],cubeVertices[7],cubeVertices[0],cubeVertices[3],cubeVertices[4],cubeVertices[7],cubeVertices[6],cubeVertices[1],cubeVertices[2],cubeVertices[5]
};

WiredCube::WiredCube(){
    logGLError();
    buildVBO();
    initMaterial();
    logGLError();
    glGenVertexArrays(1, &glInfo.vao);
    glBindVertexArray(glInfo.vao);
    setAttributes();
    glBindVertexArray(0);
    logGLError();
}

void WiredCube::buildVBO(){
    logGLError();
    glInfo.stride = 0;
    
    glInfo.size = vertices.size() * sizeof(v3d);
    glGenBuffers(1, &glInfo.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, glInfo.vbo);
    glBufferData(GL_ARRAY_BUFFER, glInfo.size, &vertices[0], GL_STATIC_DRAW);
    logGLError();
}

void WiredCube::initMaterial(){
    logGLError();
    if(!Materials::instance()->isMaterialExist("wired")){
        sp<ObjMaterial> wireMaterial = std::make_shared<ObjMaterial>("wired");
        logGLError();
        wireMaterial->program = Materials::instance()->getProgram("wired");
        wireMaterial->ambient = v4d(1, 0, 0, 1);
        logGLError();
        Materials::instance()->addMaterial(wireMaterial);
    }
    logGLError();
    material = Materials::instance()->getMaterial("wired");
}

void WiredCube::setAttributes() const{
    logGLError();
    glBindBuffer(GL_ARRAY_BUFFER, glInfo.vbo);
    unsigned char size = sizeof(v3d) / sizeof(float);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, glInfo.stride, 0);
    logGLError();
}

void WiredCube::draw() const{
    if(glInfo.vao) glBindVertexArray(glInfo.vao); else setAttributes();
    material->use();
    glDrawArrays(GL_LINE_STRIP, 0, vertices.size());
}





