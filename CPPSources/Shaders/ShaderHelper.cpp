#include "ShaderHelper.h"




#pragma mark Public
GLuint ShaderHelper::createProgram(const char *vertexSource, const char *fragmentSource){
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
    if(!vertexShader) return 0;
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);
    if(!fragmentShader) {
        glDeleteShader(vertexShader);
        return 0;
    }
    
    GLuint programObject = glCreateProgram();
    if(!programObject) {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }
    
    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    glLinkProgram(programObject);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return checkLinkStatus(programObject);
}


#pragma mark Helpers
GLuint ShaderHelper::loadShader(GLenum shaderType, const char *shaderSource){
    GLuint shader = glCreateShader(shaderType);
    if(!shader) return 0;
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    
    return checkCompileStatus(shader);
}


GLint ShaderHelper::checkCompileStatus(GLuint shader){
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if(!compiled){
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1){
            char* infoLog = (char*)malloc(sizeof(char) * infoLen);
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            esLogMessage("Error compiling shaders: \n%s\n", infoLog);
            free(infoLog);
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLint ShaderHelper::checkLinkStatus(GLuint program){
    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if(!linked){
        GLint infoLen;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1){
            char *infoLog = (char*)malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(program, infoLen, NULL, infoLog);
            esLogMessage("Error linking program: \n%s\n", infoLog);
            free(infoLog);
        }
        glDeleteProgram(program);
        return 0;
    }
    return program;
}

