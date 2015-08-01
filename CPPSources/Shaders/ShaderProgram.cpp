#include "ShaderProgram.h"

ShaderProgram::ShaderProgram( char *name, char *vertexShaderFileName, char *fragmentShaderFileName, bool debug,
              DrawCallback *drawCallback, BindAttribCallback *bindAttribCallback ){
    vertexShader = new SRShader(vertexShaderFileName, GL_VERTEX_SHADER);
    
    fragmentShader = new SRShader(fragmentShaderFileName, GL_FRAGMENT_SHADER);
    
    this->drawCallback = drawCallback;
    this->bindAttribCallback = bindAttribCallback;
    
    
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader->ID);
    glAttachShader(ID, fragmentShader->ID);
    if(bindAttribCallback) bindAttribCallback(this);
    glLinkProgram(ID);
    ShaderHelper::printProgramInfoLog(ID);
}


ShaderProgram::~ShaderProgram(){
    if(uniformArray) free(uniformArray);
    if(vertexAttribArray) free(vertexAttribArray);
    if(ID) glDeleteProgram(ID);
}