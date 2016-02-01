#pragma once

#include "GreySP.hpp"

struct DefaultDiffuseSP : public ShaderProgram{

    int modelViewMatUniLoc;
    int projectionMatUniLoc;
    int normalMatUniLoc;
    
    int lightPositionUniLoc;
    int lightColorUniLoc;
    
    int materialAmbientUniLoc;
    int materialDiffuseUniLoc;
    int materialSpecularUniLoc;
    int materialShininessUniLoc;
    
    int diffuseSamplerUniLoc;
    
    virtual void initAttribLocations();
    virtual void initUniformLocations();
    
    virtual void setUniforms(ObjMaterial *mat);
};