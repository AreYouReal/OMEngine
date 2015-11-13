#include "Shader.h"

Shader::Shader(){
    logMessage("Shader constructor! \n");
}

Shader::~Shader(){
    logMessage("Shader destructor id:%d ! \n", ID);
    deleteID();
}

void Shader::deleteID(){
    if(ID){
        glDeleteShader(ID);
        ID = 0;
    }
}
