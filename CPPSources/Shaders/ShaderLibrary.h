#pragma once

#include "SRUtils.h"
#include "Singleton.hpp"
#include "ShaderProgram.h"
#include <map>

class ShaderLibrary: public Singleton<ShaderLibrary> {
public:
    ShaderLibrary();
    ShaderLibrary(const ShaderLibrary& rhs) = delete;
    ShaderLibrary& operator=(const ShaderLibrary& rhs) = delete;
    ~ShaderLibrary();
    
    sp<ShaderProgram> getProgram(std::string name);
    sp<ShaderProgram> createProgram(const char *vertexShaderFilename, const char* fragmentShaderFilename);
    sp<ShaderProgram> createProgram(std::shared_ptr<Shader> vertexShader, std::shared_ptr<Shader> fragmentShader);
    std::shared_ptr<Shader> loadShader(GLenum shaderType, std::string shaderSource);
    
private:
    
// Fields
    
    std::map<std::string, sp<ShaderProgram>> shaders;
    
// Helper functions
    static void     printShaderInfoLog(GLuint shader);
    static void     printProgramInfoLog(GLuint program);
    static GLint    checkCompileStatus(GLuint shader);
    static GLint    checkLinkStatus(GLuint program);
    static void     printShaderInfo(GLuint shader, GLenum pname);
    static void     printShaderProgramInfo(GLuint program, GLenum pname);

};