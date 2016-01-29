
#pragma once

#include "OMUtils.h"
#include "Shader.h"
#include "ObjMaterial.h"

class ObjMaterial;

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
    virtual void setUniforms(ObjMaterial *mat);
};