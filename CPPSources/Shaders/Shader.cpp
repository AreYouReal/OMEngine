#include "Shader.h"

Shader::Shader(){
    logMessage("Shader constructor! \n");
}

Shader::~Shader(){
    deleteID();
    logMessage("Shader destructor id:%d ! \n", ID);
}

void Shader::deleteID(){
    if(ID){
        glDeleteShader(ID);
        ID = 0;
    }
}
