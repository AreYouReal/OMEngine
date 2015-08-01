#include "ShaderProgram.h"
ShaderProgram::~ShaderProgram(){
    if(uniformArray) free(uniformArray);
    if(vertexAttribArray) free(vertexAttribArray);
    if(ID) glDeleteProgram(ID);
}