
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
   
     ~ShaderProgram();
    
    
    std::string                 name;
    unsigned int                ID;
    std::vector<Uniform>        uniformArray;
    std::vector<VertexAttrib>   attribArray;
 
    BindAttribCallback  *bindAttribCallback;
    
    void                use();
    int                 getVertexAttribLocation(const char* name);
    int                 getUniformLocation(const char* name);
};