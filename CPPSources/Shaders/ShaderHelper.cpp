#include "ShaderHelper.h"
#include "main.h"

#pragma mark Public
std::shared_ptr<ShaderProgram> ShaderHelper::createProgram(const char *vertexShaderFilename,const char* fragmentShaderFilename, BindAttribCallback *bindCallback, DrawCallback *drawCallback){

    std::shared_ptr<ShaderProgram> program(new ShaderProgram());

    std::shared_ptr<Shader> vertexShader = ShaderHelper::loadShader(GL_VERTEX_SHADER, vertexShaderFilename);
    std::shared_ptr<Shader> fragmentShader = ShaderHelper::loadShader(GL_FRAGMENT_SHADER, fragmentShaderFilename);
    program->drawCallback = drawCallback;
    program->bindAttribCallback = bindCallback;
    
    program->ID = glCreateProgram();
    glAttachShader(program->ID, vertexShader->ID);
    glAttachShader(program->ID, fragmentShader->ID);
    if(program->bindAttribCallback) program->bindAttribCallback((void *)program.get());
    glLinkProgram(program->ID);
    
    int status, total, len, size;
    unsigned int type;
    unsigned short buffSize = 255;
    char name[ buffSize ];
    glGetProgramiv(program->ID, GL_LINK_STATUS, &status);
    if(!status){
        return NULL;
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

        glGetProgramiv(program->ID, GL_ACTIVE_UNIFORMS, &total);
        program->uniformArray = std::vector<Uniform>(total);
        for(unsigned int i = 0; i < total; i++){
            glGetActiveUniform(program->ID, i, buffSize, &len, &size, &type, name);
            Uniform &uniform = program->uniformArray[i];
            uniform.location = glGetUniformLocation(program->ID, name);
            uniform.name = name;
            uniform.type = type;
        }
        
    }
    glDeleteShader(vertexShader->ID);
    glDeleteShader(fragmentShader->ID);
    
    ShaderHelper::printProgramInfoLog(program->ID);
    
    return program;
}

#pragma mark Helpers
std::shared_ptr<Shader> ShaderHelper::loadShader(GLenum shaderType, const char *vertexShaderFilename){
    std::shared_ptr<Shader> shader = std::shared_ptr<Shader>(new Shader());
    shader->name = vertexShaderFilename;
    shader->type = GL_VERTEX_SHADER;
    std::unique_ptr<FileContent> shaderSource = readTextFile(SRGraphics::getAppContext(), vertexShaderFilename);
    shader->ID = glCreateShader(shaderType);
    if(!shader->ID) return nullptr;
    glShaderSource(shader->ID, 1, &shaderSource->content, NULL);
    glCompileShader(shader->ID);
    if(!checkCompileStatus(shader->ID)){
        logMessage("ERROR COMPILE SHADER! %d", shaderType);
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
