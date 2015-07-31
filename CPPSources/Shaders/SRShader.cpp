#include "SRShader.h"

#include "main.h"

SRShader::SRShader(char *fileName, unsigned int type){
    strcpy(this->name, fileName);
    this->type = type;
}

SRShader::~SRShader(){
    deleteID();
}

bool SRShader::compile(bool debug){
    if(ID) return false;
    ID = ShaderHelper::loadShader(type, readShaderFromFile(SRGraphics::GetAppContext(), name)->source);
    if(debug){ ShaderHelper::printShaderInfoLog(ID); }
    return ID;
};


void SRShader::deleteID(){
    if(ID){
        glDeleteShader(ID);
        ID = 0;
    }
}
