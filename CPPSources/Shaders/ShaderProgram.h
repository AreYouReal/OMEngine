
#pragma once

#include "SRUtils.h"
#include "Shader.h"


struct Uniform{
    char            name[ MAX_CHAR ];
    unsigned int    type;
    int             location;
    bool            constant;
};

struct VertexAttrib{
    char            name[ MAX_CHAR ];
    unsigned int    type;
    int             location;
};

typedef void(DrawCallback(void*));
typedef void(BindAttribCallback(void*));

struct ShaderProgram{
    char                name[MAX_CHAR];
    Shader              *vertexShader;
    Shader              *fragmentShader;
    unsigned int        ID;
    unsigned int        uniformCount;
    Uniform             *uniformArray;
    unsigned char       vertexAttribCount;
    VertexAttrib        *vertexAttribArray;
    
    DrawCallback        *drawCallback;
    BindAttribCallback  *bindAttribCallback;
    
    ~ShaderProgram();
    
    int                 getVertexAttribLocation(const char* name);
    int                 getUniformLocation(const char* name);
};