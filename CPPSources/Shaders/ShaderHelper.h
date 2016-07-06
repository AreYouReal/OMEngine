#pragma once

#include "OMUtils.h"
#include "Singleton.hpp"
#include "ShaderProgram.h"
#include <map>

class ShaderHelper {
public:
    
    enum ShaderType{Depth, Normal, OneColor, Skybox, SimpleGouraud, SimplePhong, Wired, GouraudMultiLight, PhongMultiLight, General };
    
    static sp<ShaderProgram>    createProgram(const string name, const string vFileName, const string fFileName, const ShaderType sType = General);
    static sp<ShaderProgram>    createProgram(const string name, const Shader &vertex, const Shader &fragment, const ShaderType sType = General);
    static Shader   loadShader(GLenum shaderType, std::string shaderFilename, std::string shaderName  = "default");
    static Shader   createShader(GLenum type, char* sourceCode, std::string shaderName);
    static ShaderType getTypeFromString(const string strType);
    
    
    static void     printShaderInfoLog(GLuint shader);
    static void     printProgramInfoLog(GLuint program);
    static GLint    checkCompileStatus(GLuint shader);
    static GLint    checkLinkStatus(GLuint program);
    static void     printShaderInfo(GLuint shader, GLenum pname);
    static void     printShaderProgramInfo(GLuint program, GLenum pname);
};