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
    string type = "type";
    string position = "position";
    string direction = "direction";
    string color = "color";
    string distance = "dst";
    string linAtten = "linAtten";
    string quadAtten = "quadAtten";
    string cosCutoff = "spotCosCutoff";
    string blend    = "spotBlend";
    
    for(int i = 0; i < numLights; ++i){
        string index = "[" + std::to_string(i) + "].";
        string lightNameIndex = lightName + index;
        
        lightLocs[i].type           = getUniformLocation(lightNameIndex + type );
        lightLocs[i].position       = getUniformLocation(lightNameIndex + position );
        lightLocs[i].direction      = getUniformLocation(lightNameIndex + direction);
        lightLocs[i].color          = getUniformLocation(lightNameIndex + color);
        lightLocs[i].distance       = getUniformLocation(lightNameIndex + distance);
        lightLocs[i].linAtten       = getUniformLocation(lightNameIndex + linAtten);
        lightLocs[i].quadAtten      = getUniformLocation(lightNameIndex + quadAtten);
        lightLocs[i].spotCosCutoff  = getUniformLocation(lightNameIndex + cosCutoff);
        lightLocs[i].spotBlend      = getUniformLocation(lightNameIndex + blend);
    }
    
    numLightUniLoc = getUniformLocation("numLights");
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
