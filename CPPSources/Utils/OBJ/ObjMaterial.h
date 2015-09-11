#pragma once

#include "SRUtils.h"
#include "ShaderProgram.h"
#include "Texture.h"

// Object material structure definition
struct ObjMaterial{
    
    void use(){
        if(program) program->use();
        
        if(tAmbient){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(tAmbient->target, tAmbient->ID);
        }
        if(tDiffuse){
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(tDiffuse->target, tDiffuse->ID);
        }
        if(tSpecular){
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(tSpecular->target, tSpecular->ID);
        }
        if(tAmbient){
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(tDisp->target, tDisp->ID);
        }
        if(tAmbient){
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(tBump->target, tBump->ID);
        }
        if(tAmbient){
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(tTranslucency->target, tTranslucency->ID);
        }
        
        if(materialDrawCalback) materialDrawCalback(this);
    }
    
    std::string name;                 // The material name!
    v4d         ambient;              // Ambient material color.
    v4d         diffuse;              // Diffuse material color.
    v4d         specular;             // Specular material color.
    v3d         transmissionFilter;   // Transmission filter of the material
    int         illuminationModel;    // ....
    float       specularExponent;     // Specular exponent (aka Hardness or Shiness).
    float       dissolve;             // The material dissolve factor a.k.a alpha
    float       opticalDensity;       // ...
    std::string mapAmbient;           // The ambient texture channel filename
    std::string mapDiffuse;           // The diffuse texture channel filename
    std::string mapSpecular;          // The specular texture channel filename
    std::string mapTranslucency;      // The translucecny....
    std::string mapDisp;              // The displacement....
    std::string mapBump;              // The bump map(aka Normal Map)
    
    std::shared_ptr<Texture> tAmbient, tDiffuse, tSpecular, tTranslucency, tDisp, tBump;
    std::shared_ptr<ShaderProgram>  program;
    DrawCallback    *materialDrawCalback;
};