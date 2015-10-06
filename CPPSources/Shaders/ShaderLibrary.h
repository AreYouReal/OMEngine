#pragma once

#include "SRUtils.h"
#include "Singleton.hpp"
#include "ShaderProgram.h"
#include <map>

class ShaderLibrary: public Singleton<ShaderLibrary> {
public:
    
    ShaderLibrary();
    ~ShaderLibrary();
    
    ShaderProgram *getProgram(std::string name);
    ShaderProgram createProgram(const char *vertexShaderFilename, const char* fragmentShaderFilename);
    
    ShaderProgram createProgram(std::shared_ptr<Shader> vertexShader, std::shared_ptr<Shader> fragmentShader);
    std::shared_ptr<Shader> loadShader(GLenum shaderType, std::string shaderSource);
    
private:
// Fields

    
    std::map<std::string, ShaderProgram> shaders;
    
// Helper functions
    static void     printShaderInfoLog(GLuint shader);
    static void     printProgramInfoLog(GLuint program);
    static GLint    checkCompileStatus(GLuint shader);
    static GLint    checkLinkStatus(GLuint program);
    static void     printShaderInfo(GLuint shader, GLenum pname);
    static void     printShaderProgramInfo(GLuint program, GLenum pname);

};