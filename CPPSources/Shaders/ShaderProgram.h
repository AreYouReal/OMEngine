
#pragma once

#include "SRUtils.h"
#include "Shader.h"

struct Uniform{
    std::string     name;
    unsigned int    type;
    int             location;
    bool            constant;
};

struct VertexAttrib{
    std::string     name;
    unsigned int    type;
    int             location;
};

typedef void(DrawCallback(void*));
typedef void(BindAttribCallback(void*));

struct ShaderProgram{
    std::string         name;
    unsigned int        ID;
    unsigned int        uniformCount;
    Uniform             *uniformArray;
    unsigned char       vertexAttribCount;
    VertexAttrib        *vertexAttribArray;
    
    DrawCallback        *drawCallback;
    BindAttribCallback  *bindAttribCallback;
    
    ~ShaderProgram();
    
    void                use();
    int                 getVertexAttribLocation(const char* name);
    int                 getUniformLocation(const char* name);
};