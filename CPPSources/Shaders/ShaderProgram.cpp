#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(){
    logMessage("Shader program constructor!\n");
}

ShaderProgram::~ShaderProgram(){
    logMessage("Shader program destructor: %d\n", ID );
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
    
    bindAttributes();
}

void ShaderProgram::bindAttributes(){
    glBindAttribLocation(ID, 0, "aPosition");
    glBindAttribLocation(ID, 1, "aNormal");
    glBindAttribLocation(ID, 2, "aTexCoord");
    glBindAttribLocation(ID, 3, "aTangent");
}