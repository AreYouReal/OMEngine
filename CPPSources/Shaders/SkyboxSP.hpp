#pragma once

#include "ShaderProgram.h"

class SkyboxSP : public ShaderProgram{
    unsigned int modelViewMatrixUniLoc;
    unsigned int projectionMatrixUniLoc;
    unsigned int cubemapSamplerLocation;

    virtual void initUniformLocations() override;
    virtual void setUniforms(const ObjMaterial *mat) override;
};