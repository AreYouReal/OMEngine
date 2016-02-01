#pragma once

#include "ShaderProgram.h"

struct NormalSP : ShaderProgram {
    
    int     modelViewMatrixUniLoc;
    int     projectionMatrixUniLoc;
    
    virtual void initAttribLocations() override;
    virtual void initUniformLocations() override;
    
//    virtual void bindAttributes() override;
    virtual void setUniforms(ObjMaterial *mat) override;
    
};