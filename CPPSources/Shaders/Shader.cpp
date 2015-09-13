#include "Shader.h"

Shader::~Shader(){
    deleteID();
}

void Shader::deleteID(){
    if(ID){
        glDeleteShader(ID);
        ID = 0;
//        logMessage("Shader destructor");
    }
}
