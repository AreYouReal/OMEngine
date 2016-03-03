#include "ShaderHelper.h"
#include "OMGame.h"

#include "NormalSP.hpp"
#include "GreySP.hpp"
#include "DefaultDiffuseSP.hpp"
#include "WireSP.hpp"

#pragma mark Public

sp<ShaderProgram> ShaderHelper::createProgram(string programName, const string vertexShaderFilename, string fragmentShaderFilename, const ShaderType sType){
    logGLError();
    Shader vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderFilename);
    logGLError();
    Shader fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderFilename);
    logGLError();
    return createProgram(programName, vertexShader, fragmentShader, sType);
}

sp<ShaderProgram> ShaderHelper::createProgram(const string programName, const Shader &vertexShader,const Shader &fragmentShader, const ShaderType sType){
    logGLError();
    
    sp<ShaderProgram> program;
    switch (sType) {
        case Normal:
            program = std::make_shared<NormalSP>();
            break;
        case Grey:
            program = std::make_shared<GreySP>();
            break;
        case DefaultPerVertex:
        case DefaultPerPixel:
            program = std::make_shared<DefaultDiffuseSP>();
            break;
        case Wired:
            program = std::make_shared<WireSP>();
            break;
        default:
            program = std::make_shared<ShaderProgram>();
            break;
    }

    logGLError();
    
    program->ID = glCreateProgram();
    
    glAttachShader(program->ID, vertexShader.ID);
    
    glAttachShader(program->ID, fragmentShader.ID);
    glLinkProgram(program->ID);
    
    int status, total, len, size;
    unsigned int type;
    unsigned short buffSize = 255;
    char name[ buffSize ];
    glGetProgramiv(program->ID, GL_LINK_STATUS, &status);
    if(!status){
        printProgramInfoLog(program->ID);
        return program;
    }else{
        glGetProgramiv(program->ID, GL_ACTIVE_ATTRIBUTES, &total);
        program->attribArray = std::vector<VertexAttrib>(total);
        for(unsigned int i = 0; i < total; i++){
            glGetActiveAttrib(program->ID, i, buffSize, &len, &size, &type, name );
            VertexAttrib &attrib = program->attribArray[i];
            attrib.location = glGetAttribLocation(program->ID, name);
            attrib.name = name;
            attrib.type = type;
        }
        logGLError();
        program->initAttribLocations();
        glGetProgramiv(program->ID, GL_ACTIVE_UNIFORMS, &total);
        program->uniformArray = std::vector<Uniform>(total);
        for(unsigned int i = 0; i < total; i++){
            glGetActiveUniform(program->ID, i, buffSize, &len, &size, &type, name);
            Uniform &uniform = program->uniformArray[i];
            uniform.location = glGetUniformLocation(program->ID, name);
            uniform.name = name;
            uniform.type = type;
        }
        program->initUniformLocations();
        logGLError();
    }
    glDeleteShader(vertexShader.ID);
    glDeleteShader(fragmentShader.ID);
    
    ShaderHelper::printProgramInfoLog(program->ID);
    program->name = programName;
    
    logGLError();
    
    return program;
}

#pragma mark Helpers
Shader ShaderHelper::loadShader(GLenum shaderType, std::string shaderFilename, std::string shaderName){
#ifdef ANDROID
    shaderFilename = "shaders/" + shaderFilename;
#endif
    std::unique_ptr<FileContent> shaderSource = readTextFile(shaderFilename);
    return createShader(shaderType, (char *)shaderSource->content, shaderName);
}

Shader ShaderHelper::createShader(GLenum type, char* sourceCode, std::string shaderName ){
    Shader shader;
    shader.name = shaderName;
    shader.type = type;
    shader.ID = glCreateShader(shader.type);
    if(!shader.ID) return shader;
    glShaderSource(shader.ID, 1, &sourceCode, NULL);
    glCompileShader(shader.ID);
    if(!checkCompileStatus(shader.ID)){
        logMessage("ERROR COMPILE SHADER! %d", shader.type);
    }
    return shader;
}

GLint ShaderHelper::checkCompileStatus(GLuint shader){
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if(!compiled){
        printShaderInfoLog(shader);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

void ShaderHelper::printShaderInfoLog(GLuint shader){
    GLint infoLen = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
    if(infoLen > 1){
        char* infoLog = (char*)malloc(sizeof(char) * infoLen);
        glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
        logMessage("Error compiling shaders: \n%s\n", infoLog);
        free(infoLog);
    }
}

GLint ShaderHelper::checkLinkStatus(GLuint program){
    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if(!linked){
        printProgramInfoLog(program);
        glDeleteProgram(program);
        return 0;
    }
    
    return program;
}

void ShaderHelper::printProgramInfoLog(GLuint program){
    GLint infoLen;
    glGetProgramiv( program, GL_INFO_LOG_LENGTH, &infoLen);
    if(infoLen > 1){
        char *infoLog = (char*)malloc(sizeof(char) * infoLen);
        glGetProgramInfoLog(program, infoLen, NULL, infoLog);
        logMessage("Error linking program: \n%s\n", infoLog);
        free(infoLog);
    }
    logGLError();
}

void ShaderHelper::printShaderInfo(GLuint shader, GLenum pname){
    GLint pStore;
    glGetShaderiv(shader, pname, &pStore);
    logMessage("Shader info for param %X  : %d\n", pname, pStore );
}

void ShaderHelper::printShaderProgramInfo(GLuint program, GLenum pname){
    GLint pStore;
    glGetProgramiv(program, pname, &pStore);
    logMessage("Program info for param %X  : %d\n", pname, pStore );
}
