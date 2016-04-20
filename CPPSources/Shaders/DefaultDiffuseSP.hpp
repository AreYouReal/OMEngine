#pragma once

#include "GreySP.hpp"

struct GouraudPhongSingleLight : public ShaderProgram{

    int modelViewMatUniLoc;
    int projectionMatUniLoc;
    int normalMatUniLoc;
    
    int lightTypeUniLoc;
    int lightPositionUniLoc;
    int lightDirUniLoc;
    int lightColorUniLoc;
    
    int materialAmbientUniLoc;
    int materialDiffuseUniLoc;
    int materialSpecularUniLoc;
    int materialShininessUniLoc;
    int materialDissolveUniLoc;
    
    int diffuseSamplerUniLoc;
    
    virtual void initAttribLocations()          override;
    virtual void initUniformLocations()         override;
    virtual void setUniforms(ObjMaterial *mat)  override;
};