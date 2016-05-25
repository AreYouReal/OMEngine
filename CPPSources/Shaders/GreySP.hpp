#pragma once

#include "NormalSP.hpp"

struct GreySP : public NormalSP{

    int normMatUniLoc;
    int lightPosUniLoc;
    int lightColorUniLoc;
    int lightTypeLocation;

    virtual void initUniformLocations() override;
    
    virtual void setUniforms(const ObjMaterial *mat) override;
    
};