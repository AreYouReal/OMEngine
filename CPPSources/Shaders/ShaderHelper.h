#pragma once

#include "SRUtils.h"
#include "ShaderProgram.h"


typedef std::shared_ptr<Shader>         SHADER;
typedef std::shared_ptr<ShaderProgram>  PROGRAM;

class ShaderHelper{
public:
    static PROGRAM createProgram(const char *vertexShaderFilename, const char* fragmentShaderFilename, BindAttribCallback *bindCallback, DrawCallback *drawCallback);
    
    static SHADER loadShader(GLenum shaderType, const char* shaderSource);
    
private:
    static void     printShaderInfoLog(GLuint shader);
    static void     printProgramInfoLog(GLuint program);
    static GLint    checkCompileStatus(GLuint shader);
    static GLint    checkLinkStatus(GLuint program);
    static void     printShaderInfo(GLuint shader, GLenum pname);
    static void     printShaderProgramInfo(GLuint program, GLenum pname);
};