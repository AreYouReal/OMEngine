#pragma once

#include "esUtil.h"

class ShaderHelper{
public:
    static GLuint createProgram(const char* vertexSource, const char* fragmentSource);
private:
    static GLint checkCompileStatus(GLuint shader);
    static GLint checkLinkStatus(GLuint program);
    static GLuint loadShader(GLenum shaderType, const char* shaderSource);
};