#include "SRUtils.h"
#include "SRShader.h"

#pragma once

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
    SRShader            *vertexShader;
    SRShader            *fragmentShader;
    unsigned int        ID;
    unsigned int        uniformCount;
    Uniform             *uniformArray;
    unsigned char       vertexAttribCount;
    VertexAttrib        *vertexAttribArray;
    
    DrawCallback        *drawCallback;
    BindAttribCallback  *bindAttribCallback;
    
    
    ShaderProgram(char *name, char *vertexShaderFileName, char *fragmentShaderFileName, bool debug,
                  DrawCallback *drawCallback, BindAttribCallback *bindAttribCallback);
    ~ShaderProgram();
    
};