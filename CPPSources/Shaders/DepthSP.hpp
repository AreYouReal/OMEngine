#include "ShaderProgram.h"

struct DepthSP : public ShaderProgram{
    // Transform
    int modelViewM = -1;
    int projectionM = -1;
    
    
    virtual void initUniformLocations() override;
    
    virtual void setUniforms(const ObjMaterial *mat) override;
};