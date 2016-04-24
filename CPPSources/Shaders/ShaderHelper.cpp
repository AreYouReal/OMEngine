#include "ShaderHelper.h"
#include "OMGame.h"

#include "NormalSP.hpp"
#include "GreySP.hpp"
#include "GouraudPhongSingleLightSP.hpp"
#include "WireSP.hpp"
#include "GPMultiLightsSP.hpp"

#pragma mark Public

std::map<string, ShaderHelper::ShaderType> enumTypeTable{
    std::pair<string, ShaderHelper::ShaderType>("Normal",           ShaderHelper::Normal            ),
    std::pair<string, ShaderHelper::ShaderType>("Grey",             ShaderHelper::Grey              ),
    std::pair<string, ShaderHelper::ShaderType>("Gouraud",          ShaderHelper::Gouraud           ),
    std::pair<string, ShaderHelper::ShaderType>("Phong",            ShaderHelper::Phong             ),
    std::pair<string, ShaderHelper::ShaderType>("Wired",            ShaderHelper::Wired             ),
    std::pair<string, ShaderHelper::ShaderType>("GourandMultiLight",ShaderHelper::GourandMultiLight ),
    std::pair<string, ShaderHelper::ShaderType>("PhongMultiLight",  ShaderHelper::PhongMultiLight   ),
    std::pair<string, ShaderHelper::ShaderType>("General",          ShaderHelper::General           )
};

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
        case Gouraud:
        case Phong:
            program = std::make_shared<GouraudPhongSingleLightSP>();
            break;
        case Wired:
            program = std::make_shared<WireSP>();
            break;
        case PhongMultiLight:
        case GourandMultiLight:
            program = std::make_shared<GPMultiLightsSP>();
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
        for(unsigned int i = 0; i < total; i++){
            glGetActiveAttrib(program->ID, i, buffSize, &len, &size, &type, name );
            VertexAttrib attrib;
            attrib.location = glGetAttribLocation(program->ID, name);
            attrib.name = name;
            attrib.type = type;
            program->attributes.insert(std::pair<string, VertexAttrib>(attrib.name, attrib));
        }
        logGLError();
        glGetProgramiv(program->ID, GL_ACTIVE_UNIFORMS, &total);
        for(unsigned int i = 0; i < total; i++){
            glGetActiveUniform(program->ID, i, buffSize, &len, &size, &type, name);
            Uniform uniform;
            uniform.location = glGetUniformLocation(program->ID, name);
            uniform.name = name;
            uniform.type = type;
            program->uniforms.insert(std::pair<string, Uniform>(uniform.name, uniform));
        }
        program->initUniformLocations();
        logGLError();
    }
    
    ShaderHelper::printProgramInfoLog(program->ID);
    program->name = programName;
    
    logGLError();
    
    return program;
}

ShaderHelper::ShaderType ShaderHelper::getTypeFromString(const string strType){
    if(enumTypeTable.find(strType) != enumTypeTable.end()){
        return enumTypeTable[strType];
    }
    return ShaderHelper::ShaderType::Normal;
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
