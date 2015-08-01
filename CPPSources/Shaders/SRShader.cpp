#include "SRShader.h"

#include "main.h"

SRShader::SRShader(char *fileName, unsigned int type){
    strcpy(this->name, fileName);
    this->type = type;
    this->ID = 0;
    compile(true);
}

SRShader::~SRShader(){
    deleteID();
}

bool SRShader::compile(bool debug){
    logMessage("ID: %d \n", ID);
    if(ID) return false;
    char* source = readTextFile(SRGraphics::GetAppContext(), name);
    ID = ShaderHelper::loadShader(type, source);
    if(source) delete[] source;
    if(debug){ ShaderHelper::printShaderInfoLog(ID); }
    logMessage("Shader id: %d", ID);
    return ID;
};


void SRShader::deleteID(){
    if(ID){
        glDeleteShader(ID);
        ID = 0;
    }
}
