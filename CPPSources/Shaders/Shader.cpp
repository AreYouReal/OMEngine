#include "Shader.h"
#include "main.h"

Shader::~Shader(){
    deleteID();
}

void Shader::deleteID(){
    if(ID){
        glDeleteShader(ID);
        ID = 0;
    }
}
