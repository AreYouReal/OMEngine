#include "GouraudMultiLightSP.hpp"


void GouraudMultiLightSP::initUniformLocations(){
    initTransformUniformLocations();
    initMaterialUniformLocations();
    initLightUniLocations();
}


void GouraudMultiLightSP::setUniforms(const ObjMaterial *mat){
    setTransformUniforms();
    setMaterialUniforms(mat);
    setLightUniforms();
}


void GouraudMultiLightSP::initLightUniLocations(){
    string lightName = "uLight";

    
    for(int i = 0; i < numLights; ++i){
        LightUniformLocations lightUniLocs;
        string index = "[" + std::to_string(i) + "].";
        string lightNameIndex = lightName + index;
        
        lightUniLocs.type           = getUniformLocation(lightNameIndex + ShaderProgram::type );
        lightUniLocs.position       = getUniformLocation(lightNameIndex + ShaderProgram::position );
        lightUniLocs.direction      = getUniformLocation(lightNameIndex + ShaderProgram::direction);
        lightUniLocs.color          = getUniformLocation(lightNameIndex + ShaderProgram::color);
        lightUniLocs.distance       = getUniformLocation(lightNameIndex + ShaderProgram::distance);
        lightUniLocs.linAtten       = getUniformLocation(lightNameIndex + ShaderProgram::linAtten);
        lightUniLocs.quadAtten      = getUniformLocation(lightNameIndex + ShaderProgram::quadAtten);
        lightUniLocs.spotCosCutoff  = getUniformLocation(lightNameIndex + ShaderProgram::cosCutoff);
        lightUniLocs.spotBlend      = getUniformLocation(lightNameIndex + ShaderProgram::blend);
        
        lightLocs.push_back(lightUniLocs);
    }
    
    numLightUniLoc = getUniformLocation(ShaderProgram::numLights);
}

void GouraudMultiLightSP::setLightUniforms(){
    glUniform1i(numLightUniLoc, numLights);
    for(int i = 0; i < numLights; ++i){
        sp<LightSource> light = Illuminator::instance()->getLightSource(i);
        if(light != nullptr){
            setUniformForLightSource(light, i);
        }
    }
}

void GouraudMultiLightSP::setUniformForLightSource(sp<LightSource> light, int index){
    glUniform1i(lightLocs[index].type, light->type());
    v3d temp = light->getDirectionInEyeSpace();
    glUniform3fv(lightLocs[index].direction, 1, &temp.x);
    glUniform4fv(lightLocs[index].color, 1, &light->getColor().x);
    switch (light->type()) {
        case LightSource::Type::DIRECTION:
            break;
        case LightSource::Type::POINT:
        {
            v3d temp = light->getPositionInEyeSpace();
            glUniform3fv(lightLocs[index].position, 1, &temp.x);
            glUniform1f(lightLocs[index].distance,      light->distance());
            glUniform1f(lightLocs[index].linAtten,      light->linearAtten());
            glUniform1f(lightLocs[index].quadAtten,     light->quadAtten());
        }
            break;
        case LightSource::Type::SPOT:
        {
            glUniform1f(lightLocs[index].spotCosCutoff, light->spotCosCutoff);
            glUniform1f(lightLocs[index].spotBlend,     light->spotBlend);
        }
            break;
        default:
            break;
    }
   
}
