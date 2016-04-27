#include "ShaderProgram.h"


struct GouraudMultiLightSP : public ShaderProgram{
    uint maxLight = 3;
    
    TransformUniformLocations           transformLoc;
    MaterialUniformLocations            matLoc;
    TextureUniformLocations             texLoc;
    std::vector<LightUniformLocations>  lightLoc;

    virtual void initUniformLocations() override;
    virtual void setUniforms(const ObjMaterial *mat) override;
};