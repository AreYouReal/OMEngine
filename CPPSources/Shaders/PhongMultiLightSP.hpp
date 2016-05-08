#include "ShaderProgram.h"

const int MAX_LIGHTS = 5;

struct PhongMultiLightSP : ShaderProgram{
    uint numLights = 2;
    
    int numLightUniLoc = -1;
    
    std::vector<LightUniformLocations>  vsLightLoc;
    std::vector<LightUniformLocations>  fsLightLoc;
    
    
    virtual void initUniformLocations() override;
    virtual void setUniforms(const ObjMaterial *mat) override;
    
    
    void initLightUniLocations();
    void setLightUniforms();
    void setUniformForLightSource(sp<LightSource> light, int index);
};