#pragma once

#include "SRUtils.h"
#include "ShaderProgram.h"
#include "Texture.h"

// Object material structure definition
struct ObjMaterial{
    
    ObjMaterial(std::string name = "default");
    
    void use();
    
    std::string name;                                   // The material name!
    v4d         ambient     {0.1f, 0.1f, 0.1f, 1.0f};   // Ambient material color.
    v4d         diffuse     {0.5f, 0.5f, 0.5f, 1.0f};   // Diffuse material color.
    v4d         specular    {1.0f, 1.0f, 1.0f, 1.0f};   // Specular material color.
    v3d         transmissionFilter{0.1f, 0.1f, 0.1f};   // Transmission filter of the material
    int         illuminationModel{0};                   // ....
    float       specularExponent{90.0};                 // Specular exponent (aka Hardness or Shiness).
    float       dissolve{1.0f};                         // The material dissolve factor a.k.a alpha
    float       opticalDensity{1.0f};                   // ...
    std::string mapAmbient;           // The ambient texture channel filename
    std::string mapDiffuse;           // The diffuse texture channel filename
    std::string mapSpecular;          // The specular texture channel filename
    std::string mapTranslucency;      // The translucecny....
    std::string mapDisp;              // The displacement....
    std::string mapBump;              // The bump map(aka Normal Map)
    
    std::shared_ptr<Texture> tAmbient, tDiffuse, tSpecular, tTranslucency, tDisp, tBump;
    std::shared_ptr<ShaderProgram>  program;
    
    virtual void setUniforms();
};