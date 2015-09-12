#pragma once

#include "SRUtils.h"
#include "ShaderProgram.h"
#include "Typedefs.h"



class ShaderHelper{
public:
    static std::shared_ptr<ShaderProgram> createProgram(const char *vertexShaderFilename, const char* fragmentShaderFilename, BindAttribCallback *bindCallback, DrawCallback *drawCallback);
    
    static std::shared_ptr<Shader> loadShader(GLenum shaderType, const char* shaderSource);
    
private:
    static void     printShaderInfoLog(GLuint shader);
    static void     printProgramInfoLog(GLuint program);
    static GLint    checkCompileStatus(GLuint shader);
    static GLint    checkLinkStatus(GLuint program);
    static void     printShaderInfo(GLuint shader, GLenum pname);
    static void     printShaderProgramInfo(GLuint program, GLenum pname);
};