
#pragma once

#include "OMUtils.h"
#include "Shader.h"
#include "ObjMaterial.h"
#include "Camera.h"
#include "Illuminator.hpp"


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

    virtual void initUniformLocations();
    
    virtual void setUniforms(ObjMaterial *mat);
    
    
    // Attributes
    static const string posAttribName;
    static const string normAttribName;
    static const string texCoordAttribName;
    static const string tanAttribName;
    // Uniforms
    static const string uniModelViewMatName;
    static const string uniProjectionMatName;
    static const string uniNormalMName;
    static const string uniShadowMName;
};