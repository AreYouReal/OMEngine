#pragma once

#include "NormalSP.hpp"

struct OneColorSP : public NormalSP{

    int normMatUniLoc;
    int lightPosUniLoc;
    int lightColorUniLoc;
    int lightTypeLocation;
    int colorLocation;
    
    virtual void initUniformLocations() override;
    
    virtual void setUniforms(const ObjMaterial *mat) override;
    
};