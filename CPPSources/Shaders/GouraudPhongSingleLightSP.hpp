#pragma once

#include "GreySP.hpp"

struct GouraudPhongSingleLightSP : public ShaderProgram{

    int modelViewMatUniLoc = -1;
    int projectionMatUniLoc = -1;
    int normalMatUniLoc = -1;
    int shadowMatUniLoc = -1;
    
    int lightTypeUniLoc = -1;
    int lightPositionUniLoc = -1;
    int lightDirUniLoc = -1;
    int lightColorUniLoc = -1;
    
    int materialAmbientUniLoc = -1;
    int materialDiffuseUniLoc = -1;
    int materialSpecularUniLoc = -1;
    int materialShininessUniLoc = -1;
    int materialDissolveUniLoc = -1;
    
    int diffuseSamplerUniLoc = -1;
    int bumpSamplerUniLoc = -1;
    int ambientSamplerDiffuse = -1;

    virtual void initUniformLocations()         override;
    virtual void setUniforms(ObjMaterial *mat)  override;
};