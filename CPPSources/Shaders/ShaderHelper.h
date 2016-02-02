#pragma once

#include "OMUtils.h"
#include "Singleton.hpp"
#include "ShaderProgram.h"
#include <map>

class ShaderHelper {
public:
    
    enum ShaderType{Normal, Grey, DefaultPerVertex, DefaultPerPixel, Wired, Other };
    
    static sp<ShaderProgram>    createProgram(const string name, const string vFileName, const string fFileName, const ShaderType sType = Other);
    static sp<ShaderProgram>    createProgram(const string name, const Shader &vertex, const Shader &fragment, const ShaderType sType = Other);
    static Shader   loadShader(GLenum shaderType, std::string shaderFilename, std::string shaderName  = "default");
    static Shader   createShader(GLenum type, char* sourceCode, std::string shaderName);
    static void     printShaderInfoLog(GLuint shader);
    static void     printProgramInfoLog(GLuint program);
    static GLint    checkCompileStatus(GLuint shader);
    static GLint    checkLinkStatus(GLuint program);
    static void     printShaderInfo(GLuint shader, GLenum pname);
    static void     printShaderProgramInfo(GLuint program, GLenum pname);
};