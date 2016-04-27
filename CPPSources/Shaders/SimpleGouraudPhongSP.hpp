#include "ShaderProgram.h"

struct SimpleGouraudPhongSP : public ShaderProgram{
#pragma mark Uniforms locations

    LightUniformLocations       lightLoc;
    
#pragma mark Overrides
    virtual void initUniformLocations() override;
    virtual void setUniforms(const ObjMaterial *mat) override;

    void initLightUniLocations();
    
    void setLightUniforms();
    void setMaterialUniforms(const ObjMaterial *mat);
};