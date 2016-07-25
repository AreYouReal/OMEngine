#include "ShaderProgram.h"

struct GouraudMultiLightSP : public ShaderProgram{
    uint numLights = 2;
    int numLightUniLoc = -1;
    
    std::vector<LightUniformLocations>  lightLocs;

    virtual void initUniformLocations() override;
    virtual void setUniforms(const ObjMaterial *mat) override;
    
    
    void initLightUniLocations();
    void setLightUniforms();
    void setUniformForLightSource(LightSource* light, int index);
};