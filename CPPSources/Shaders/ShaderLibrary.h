#pragma once

#include "SRUtils.h"
#include "ShaderProgram.h"
#include "Typedefs.h"
#include <map>

class ShaderLibrary{
public:
    static std::shared_ptr<ShaderLibrary> instance();
    
    ShaderLibrary();

    std::shared_ptr<ShaderProgram> getProgram(std::string name);
    std::shared_ptr<ShaderProgram> createProgram(const char *vertexShaderFilename, const char* fragmentShaderFilename, BindAttribCallback *bindCallback, DrawCallback *drawCallback);
    std::shared_ptr<Shader> loadShader(GLenum shaderType, std::string shaderSource);
    
private:
    
    static std::shared_ptr<ShaderLibrary> mInstance;
// Fields
    std::map<std::string, std::shared_ptr<ShaderProgram>> shaders;
    
// Helper functions
    static void     printShaderInfoLog(GLuint shader);
    static void     printProgramInfoLog(GLuint program);
    static GLint    checkCompileStatus(GLuint shader);
    static GLint    checkLinkStatus(GLuint program);
    static void     printShaderInfo(GLuint shader, GLenum pname);
    static void     printShaderProgramInfo(GLuint program, GLenum pname);
};