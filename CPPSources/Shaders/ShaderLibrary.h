#pragma once

#include "SRUtils.h"
#include "ShaderProgram.h"
#include "Typedefs.h"
#include <map>

class ShaderLibrary{
public:
    static ShaderLibrary *instance();
    
    ShaderLibrary();
    ~ShaderLibrary();

    ShaderProgram *getProgram(std::string name);
    ShaderProgram createProgram(const char *vertexShaderFilename, const char* fragmentShaderFilename);
    
    ShaderProgram createProgram(std::shared_ptr<Shader> vertexShader, std::shared_ptr<Shader> fragmentShader);
    std::shared_ptr<Shader> loadShader(GLenum shaderType, std::string shaderSource);
    
private:
// Fields
    
    static ShaderLibrary *mInstance;
    
    std::map<std::string, ShaderProgram> shaders;
    
// Helper functions
    static void     printShaderInfoLog(GLuint shader);
    static void     printProgramInfoLog(GLuint program);
    static GLint    checkCompileStatus(GLuint shader);
    static GLint    checkLinkStatus(GLuint program);
    static void     printShaderInfo(GLuint shader, GLenum pname);
    static void     printShaderProgramInfo(GLuint program, GLenum pname);
};