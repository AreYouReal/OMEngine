#include "Shader.h"

Shader::Shader(){
}

Shader::~Shader(){
    deleteID();
}

void Shader::deleteID(){
    if(ID){
        glDeleteShader(ID);
        ID = 0;
    }
}
