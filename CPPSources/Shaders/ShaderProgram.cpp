#include "ShaderProgram.h"
ShaderProgram::~ShaderProgram(){
    if(ID) glDeleteProgram(ID);
    logMessage("Shader program destructor: %d", ID );
}


int ShaderProgram::getUniformLocation(const char *name){
    for(int i = 0; i < uniformArray.size(); i++) if(!strcmp(uniformArray[i].name.c_str(), name)) return uniformArray[i].location;
    
    return -1;
}

int ShaderProgram::getVertexAttribLocation(const char *name){
    for(int i = 0; i < attribArray.size(); i++) if(!strcmp(attribArray[i].name.c_str(), name)) return attribArray[i].location;
    
    return -1;
}

void ShaderProgram::use(){
    glUseProgram(ID);
    
    if(bindAttribCallback) bindAttribCallback((void*)this);
}