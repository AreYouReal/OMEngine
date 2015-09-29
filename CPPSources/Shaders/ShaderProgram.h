
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

struct ShaderProgram{
    ShaderProgram();
     ~ShaderProgram();
    
    
    std::string                 name;
    unsigned int                ID;
    std::vector<Uniform>        uniformArray;
    std::vector<VertexAttrib>   attribArray;
 
    
    void                use();
    int                 getVertexAttribLocation(const char* name);
    int                 getUniformLocation(const char* name);
    
    virtual void bindAttributes();
};