#include "ShaderLibrary.h"
#include "Game.h"

#pragma mark Public

typedef std::shared_ptr<Shader>         SHADER;
typedef std::shared_ptr<ShaderProgram>  PROGRAM;

std::weak_ptr<ShaderLibrary> mInstance;

std::shared_ptr<ShaderLibrary> ShaderLibrary::instance(){
    if(mInstance.expired()){
        std::shared_ptr<ShaderLibrary> shared = std::shared_ptr<ShaderLibrary>(new ShaderLibrary());
        mInstance = std::weak_ptr<ShaderLibrary>(shared);
        return shared;
    }
    return std::shared_ptr<ShaderLibrary>(mInstance);
}

ShaderLibrary::ShaderLibrary(){
    logMessage("ShaderLibrary constructor!\n");
    // Default program
    // Use normal as color in shader
    std::shared_ptr<Shader> vertexShader = loadShader(GL_VERTEX_SHADER, "pos_norm_vertex.glsl");
    std::shared_ptr<Shader> fragmentShader = loadShader(GL_FRAGMENT_SHADER, "norm_as_color_fragment.glsl");
    ShaderProgram program = createProgram(vertexShader, fragmentShader);
    shaders.insert(std::pair<std::string, ShaderProgram>("norm_as_color", program));
    //--------------------------------------------------------
    // Per vertex lighting
    std::shared_ptr<Shader> defVShader = loadShader(GL_VERTEX_SHADER, "default_gray_vertex.glsl");
    std::shared_ptr<Shader> defFShader = loadShader(GL_FRAGMENT_SHADER, "default_gray_fragment.glsl");
    ShaderProgram defProg = createProgram(defVShader, defFShader);
    shaders.insert(std::pair<std::string, ShaderProgram>("default_gray", defProg));
    //--------------------------------------------------------
    // Per vertex lighting
    SHADER vertexPerVertexLighting = loadShader(GL_VERTEX_SHADER, "vertexPerVertex.glsl");
    SHADER fragmentPerVertexLighting = loadShader(GL_FRAGMENT_SHADER, "fragmentPerVertex.glsl");
    ShaderProgram perVertexProgram = createProgram(vertexPerVertexLighting, fragmentPerVertexLighting);
    shaders.insert(std::pair<std::string, ShaderProgram>("defaultPerVertex", perVertexProgram));
    //--------------------------
    // Solid, alpha tested and transparent programs with per pixel lighting.
    std::shared_ptr<Shader> vShader = loadShader(GL_VERTEX_SHADER, "vertexPerPixel.glsl");
    std::shared_ptr<Shader> fSolidShader = loadShader(GL_FRAGMENT_SHADER, "fragmentPerPixel.glsl");
    ShaderProgram solidProgram = createProgram(vShader, fSolidShader);
    shaders.insert(std::pair<std::string, ShaderProgram>("defaultPerPixel", solidProgram));
    //--------------------------
    // Bump shader
    SHADER vertexBump = loadShader(GL_VERTEX_SHADER, "vertexBump.glsl");
    SHADER fragmentBump = loadShader(GL_FRAGMENT_SHADER, "fragmentBump.glsl");
    ShaderProgram bumpProgram = createProgram(vertexBump, fragmentBump);
    shaders.insert(std::pair<std::string, ShaderProgram>("bump", bumpProgram));
    //---------------------------------------------------------
}

ShaderLibrary::~ShaderLibrary(){
    logMessage("ShaderLibrary destructor!\n");
}

ShaderProgram* ShaderLibrary::getProgram(std::string name){
    if(shaders.find(name) == shaders.end()) return nullptr;
    return &shaders[name];
}

ShaderProgram ShaderLibrary::createProgram(const char *vertexShaderFilename,const char* fragmentShaderFilename){
    std::shared_ptr<Shader> vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderFilename);
    std::shared_ptr<Shader> fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderFilename);
    return createProgram(vertexShader, fragmentShader);
}

ShaderProgram ShaderLibrary::createProgram(std::shared_ptr<Shader> vertexShader, std::shared_ptr<Shader> fragmentShader){
    ShaderProgram program;
    program.ID = glCreateProgram();
    glAttachShader(program.ID, vertexShader->ID);
    glAttachShader(program.ID, fragmentShader->ID);
    glLinkProgram(program.ID);
    
    int status, total, len, size;
    unsigned int type;
    unsigned short buffSize = 255;
    char name[ buffSize ];
    glGetProgramiv(program.ID, GL_LINK_STATUS, &status);
    if(!status){
        printProgramInfoLog(program.ID);
        return program;
    }else{
        glGetProgramiv(program.ID, GL_ACTIVE_ATTRIBUTES, &total);
        program.attribArray = std::vector<VertexAttrib>(total);
        for(unsigned int i = 0; i < total; i++){
            glGetActiveAttrib(program.ID, i, buffSize, &len, &size, &type, name );
            VertexAttrib &attrib = program.attribArray[i];
            attrib.location = glGetAttribLocation(program.ID, name);
            attrib.name = name;
            attrib.type = type;
        }
        
        glGetProgramiv(program.ID, GL_ACTIVE_UNIFORMS, &total);
        program.uniformArray = std::vector<Uniform>(total);
        for(unsigned int i = 0; i < total; i++){
            glGetActiveUniform(program.ID, i, buffSize, &len, &size, &type, name);
            Uniform &uniform = program.uniformArray[i];
            uniform.location = glGetUniformLocation(program.ID, name);
            uniform.name = name;
            uniform.type = type;
        }
        
    }
    glDeleteShader(vertexShader->ID);
    glDeleteShader(fragmentShader->ID);
    
    ShaderLibrary::printProgramInfoLog(program.ID);
    
    return program;
}

#pragma mark Helpers
std::shared_ptr<Shader> ShaderLibrary::loadShader(GLenum shaderType, std::string vertexShaderFilename){
#ifdef ANDROID
    vertexShaderFilename = "shaders/" + vertexShaderFilename;
#endif
    std::shared_ptr<Shader> shader = std::shared_ptr<Shader>(new Shader());
    shader->name = vertexShaderFilename;
    shader->type = GL_VERTEX_SHADER;
    std::unique_ptr<FileContent> shaderSource = readTextFile(Game::getAppContext(), vertexShaderFilename);
    shader->ID = glCreateShader(shaderType);
    if(!shader->ID) return nullptr;
    glShaderSource(shader->ID, 1, &shaderSource->content, NULL);
    glCompileShader(shader->ID);
    if(!checkCompileStatus(shader->ID)){
        logMessage("ERROR COMPILE SHADER! %d", shaderType);
    }
    return shader;
}

GLint ShaderLibrary::checkCompileStatus(GLuint shader){
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if(!compiled){
        printShaderInfoLog(shader);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

void ShaderLibrary::printShaderInfoLog(GLuint shader){
    GLint infoLen = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
    if(infoLen > 1){
        char* infoLog = (char*)malloc(sizeof(char) * infoLen);
        glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
        logMessage("Error compiling shaders: \n%s\n", infoLog);
        free(infoLog);
    }
}

GLint ShaderLibrary::checkLinkStatus(GLuint program){
    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if(!linked){
        printProgramInfoLog(program);
        glDeleteProgram(program);
        return 0;
    }
    
    return program;
}

void ShaderLibrary::printProgramInfoLog(GLuint program){
    GLint infoLen;
    glGetProgramiv( program, GL_INFO_LOG_LENGTH, &infoLen);
    if(infoLen > 1){
        char *infoLog = (char*)malloc(sizeof(char) * infoLen);
        glGetProgramInfoLog(program, infoLen, NULL, infoLog);
        logMessage("Error linking program: \n%s\n", infoLog);
        free(infoLog);
    }
}

void ShaderLibrary::printShaderInfo(GLuint shader, GLenum pname){
    GLint pStore;
    glGetShaderiv(shader, pname, &pStore);
    logMessage("Shader info for param %X  : %d\n", pname, pStore );
}

void ShaderLibrary::printShaderProgramInfo(GLuint program, GLenum pname){
    GLint pStore;
    glGetProgramiv(program, pname, &pStore);
    logMessage("Program info for param %X  : %d\n", pname, pStore );
}
