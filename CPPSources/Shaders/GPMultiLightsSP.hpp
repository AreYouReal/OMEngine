#include "ShaderProgram.h"

class GPMultiLightsSP : public ShaderProgram{
    
    // Transform
    int uModelViewMLoc  = -1;
    int uProjectionMLoc = -1;
    int uNormalMLoc     = -1;
    int uShadowMLoc     = -1; // If receive shadows set to TRUE
    
    // Samplers
    int uSamplerAmbient = -1;   // For now using for shadow texture (depth texture) if receive shadows is enabled.
    int uSamplerDiffuse = -1;
    int uSamplerBump    = -1;
    
    
    
    virtual void initUniformLocations() override;
    
    virtual void setUniforms(const ObjMaterial *mat) override;
    
};