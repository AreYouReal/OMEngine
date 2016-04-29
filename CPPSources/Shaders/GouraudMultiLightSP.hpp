#include "ShaderProgram.h"

#define MAX_LIGHTS 5

struct GouraudMultiLightSP : public ShaderProgram{
    uint numLights = 2;

    LightUniformLocations lightLocs[MAX_LIGHTS];
    int numLightUniLoc = -1;
    
    std::vector<LightUniformLocations>  lightLoc;

    virtual void initUniformLocations() override;
    virtual void setUniforms(const ObjMaterial *mat) override;
    
    
    void initLightUniLocations();
    void setLightUniforms();
    void setUniformForLightSource(sp<LightSource> light, int index);
};