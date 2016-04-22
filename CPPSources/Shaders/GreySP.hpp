#pragma once

#include "NormalSP.hpp"

struct GreySP : public NormalSP{

    int normMatUniLoc;
    int lightPosUniLoc;
    int lightColorUniLoc;

    virtual void initUniformLocations();
    
    virtual void setUniforms(ObjMaterial *mat);
    
};