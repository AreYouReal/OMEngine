#pragma once

#include "ShaderProgram.h"

struct WireSP : public ShaderProgram{

    int modelViewMatUniLoc;
    int projectionMatUniLoc;
    int materialAmbient;
    
    virtual void initUniformLocations()         override;
    virtual void setUniforms(const ObjMaterial *mat)  override;
};