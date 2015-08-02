#include "ShaderProgram.h"
ShaderProgram::~ShaderProgram(){
    if(uniformArray) free(uniformArray);
    if(vertexAttribArray) free(vertexAttribArray);
    if(ID) glDeleteProgram(ID);
    
    logMessage("Shader program destructor");
}


int ShaderProgram::getUniformLocation(const char *name){
    for(int i = 0; i < uniformCount; i++) if(!strcmp(uniformArray[i].name, name)) return uniformArray[i].location;
    
    return -1;
}

int ShaderProgram::getVertexAttribLocation(const char *name){
    for(int i = 0; i < vertexAttribCount; i++) if(!strcmp(vertexAttribArray[i].name, name)) return vertexAttribArray[i].location;
    
    return -1;
}