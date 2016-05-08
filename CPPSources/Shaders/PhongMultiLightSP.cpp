#include "PhongMultiLightSP.hpp"



void PhongMultiLightSP::initUniformLocations(){
    initTransformUniformLocations();
    initMaterialUniformLocations();
    initLightUniLocations();
}


void PhongMultiLightSP::initLightUniLocations(){
    string vsLightName{"uLightVS"};
    string fsLightName{"uLightFS"};
    
    for(int i = 0; i < numLights; ++i){
        string index = "[" + std::to_string(i) + "].";
        string vsLightNameIndex = vsLightName + index;
        
        LightUniformLocations vsLightUniLocs;
        vsLightUniLocs.type           = getUniformLocation(vsLightNameIndex + ShaderProgram::type );
        vsLightUniLocs.position       = getUniformLocation(vsLightNameIndex + ShaderProgram::position );
        vsLightUniLocs.direction      = getUniformLocation(vsLightNameIndex + ShaderProgram::direction);
        vsLightLoc.push_back(vsLightUniLocs);
        
        
        string fsLightNameIndex = fsLightName + index;
        
        LightUniformLocations fsLightUniLocs;
        fsLightUniLocs.type           = getUniformLocation(fsLightNameIndex + ShaderProgram::type);
        fsLightUniLocs.color          = getUniformLocation(fsLightNameIndex + ShaderProgram::color);
        fsLightUniLocs.distance       = getUniformLocation(fsLightNameIndex + ShaderProgram::distance);
        fsLightUniLocs.linAtten       = getUniformLocation(fsLightNameIndex + ShaderProgram::linAtten);
        fsLightUniLocs.quadAtten      = getUniformLocation(fsLightNameIndex + ShaderProgram::quadAtten);
        fsLightUniLocs.spotCosCutoff  = getUniformLocation(fsLightNameIndex + ShaderProgram::cosCutoff);
        fsLightUniLocs.spotBlend      = getUniformLocation(fsLightNameIndex + ShaderProgram::blend);
        fsLightLoc.push_back(fsLightUniLocs);
    }
    
    numLightUniLoc = getUniformLocation(ShaderProgram::numLights);
}


void PhongMultiLightSP::setUniforms(const ObjMaterial *mat){
    setTransformUniforms();
    setMaterialUniforms(mat);
    setLightUniforms();
}

void PhongMultiLightSP::setLightUniforms(){
    for(int i = 0; i < numLights; ++i){
        sp<LightSource> light = Illuminator::instance()->getLightSource(i);
        if(light != nullptr){
            setUniformForLightSource(light, i);
        }
    }
}

void PhongMultiLightSP::setUniformForLightSource(sp<LightSource> light, int index){
    glUniform1i(vsLightLoc[index].type, light->type());
    glUniform1i(fsLightLoc[index].type, light->type());
    
    v3d temp = light->getDirectionInEyeSpace();
    glUniform3fv(vsLightLoc[index].direction, 1, &temp.x);
    glUniform4fv(fsLightLoc[index].color, 1, &light->getColor().x);
    switch (light->type()) {
        case LightSource::Type::DIRECTION:
            break;
        case LightSource::Type::POINT:
        {
            v3d temp = light->getPositionInEyeSpace();
            glUniform3fv(vsLightLoc[index].position, 1, &temp.x);
            glUniform1f(fsLightLoc[index].distance,      light->distance());
            glUniform1f(fsLightLoc[index].linAtten,      light->linearAtten());
            glUniform1f(fsLightLoc[index].quadAtten,     light->quadAtten());
        }
            break;
        case LightSource::Type::SPOT:
        {
            glUniform1f(fsLightLoc[index].spotCosCutoff, light->spotCosCutoff);
            glUniform1f(fsLightLoc[index].spotBlend,     light->spotBlend);
        }
            break;
        default:
            break;
    }
}
