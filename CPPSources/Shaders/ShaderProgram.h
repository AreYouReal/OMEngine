
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

struct MaterialUniformLocations{
    int ambient = -1;
    int diffuse = -1;
    int specular = -1;
    int shininess = -1;
};

struct TransformUniformLocations{
    int modelViewMat = -1;
    int projectionMat = -1;
    int normalMat = -1;
    
    int shadowMat = -1;
    int projectorMat = -1;
};

struct LightUniformLocations{
    int type            = -1;
    int position        = -1;
    int direction       = -1;

    int color           = -1;
    
    int distance        = -1;
    int linAtten        = -1;
    int quadAtten       = -1;
    
    int spotCosCutoff   = -1;
    int spotBlend       = -1;

};

struct TextureUniformLocations{
    int diffuse     = -1;
    int bump        = -1;
    int shadow      = -1;
};

struct ShaderProgram{
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
    // LIGHT related
    static const string type;
    static const string position;
    static const string direction;
    static const string color;
    static const string distance;
    static const string linAtten;
    static const string quadAtten;
    static const string cosCutoff;
    static const string blend;
    
    static const string numLights;
 
    // Uniform locations
    TransformUniformLocations transformLoc;
    MaterialUniformLocations  matLoc;
    TextureUniformLocations   texLoc;
    
    ShaderProgram();
     ~ShaderProgram();
    
    // Fields
    std::string                 name;
    unsigned int                ID;
    std::map<string, Uniform>        uniforms;
    std::map<string, VertexAttrib>   attributes;
 
    // USE
    void                use();

    // Virtuals
    virtual void initUniformLocations();
    virtual void setUniforms(const ObjMaterial *mat);

    // General
    int getUniformLocation(const string &name);
    
protected:

    // Helpers
    void initTransformUniformLocations();
    void initMaterialUniformLocations();
    
    void setTransformUniforms();

    void setMaterialUniforms(const ObjMaterial *mat);
    
};