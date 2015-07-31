#pragma once

#include "SRUtils.h"

class ShaderHelper{
public:
    static GLuint createProgram(const char* vertexSource, const char* fragmentSource);
    static GLuint loadShader(GLenum shaderType, const char* shaderSource);
    
    static void printShaderInfoLog(GLuint shader);
    static void printProgramInfoLog(GLuint program);
private:
    static GLint checkCompileStatus(GLuint shader);
    static GLint checkLinkStatus(GLuint program);

    static void printShaderInfo(GLuint shader, GLenum pname);
    static void printShaderProgramInfo(GLuint program, GLenum pname);
};