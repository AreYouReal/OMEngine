#pragma once

#include "ShaderProgram.h"

struct NormalSP : ShaderProgram {

    
    
    
    virtual void initAttribLocations();
    virtual void initUniformLocations();
    
    virtual void bindAttributes();
    virtual void setUniforms(ObjMaterial *mat);
    
}