#include "ShaderProgram.h"

struct SimpleGouraudSP : public ShaderProgram{
#pragma mark Uniforms locations
    // Transform related
    int modelViewM  = -1;
    int projectionM = -1;
    int normalM     = -1;
    
    // Light related
    int lightType   = -1;
    int lightPos    = -1;
    int lightDir    = -1;
    int lightColor  = -1;
    
    // Material related
    int matAmbientColor     = -1;
    int matDiffuseColor     = -1;
    int matSpecularColor    = -1;
    int matShininess        = -1;
    
#pragma mark Overrides
    virtual void initUniformLocations() override;
    virtual void setUniforms(const ObjMaterial *mat) override;

    void initTransformUniLocations();
    void initLightUniLocations();
    void initMaterialUniLocations();
    
    void setTransformUniforms();
    void setLightUniforms();
    void setMaterialUniforms(const ObjMaterial *mat);
};