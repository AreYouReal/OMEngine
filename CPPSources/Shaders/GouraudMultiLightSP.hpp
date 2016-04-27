#include "ShaderProgram.h"


struct GouraudMultiLightSP : public ShaderProgram{
    uint maxLight = 3;
    
    std::vector<LightUniformLocations>  lightLoc;

    virtual void initUniformLocations() override;
    virtual void setUniforms(const ObjMaterial *mat) override;
    
    
    void initLightUniLocations();
};