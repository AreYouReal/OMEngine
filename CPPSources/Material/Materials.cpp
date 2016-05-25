
#include "Materials.hpp"
#include "OMUtils.h"
#include "ShaderHelper.h"

static std::map<string, string>  meshShaderTable{
    std::pair<string, string>("bblock", "Grey"),
    std::pair<string, string>("actionArrow", "Normal"),
    std::pair<string, string>("minimon", "phongMultiLight.omg"),
};

Materials::Materials(){
    loadPrograms();
    loadMaterial("minimon.mtl");
    loadOMGFile("phongMultiLight.omg");
}

Materials::~Materials(){
    materials.clear();
}

bool Materials::loadMaterial(const std::string &name){
    logGLError();
    if(materials.find(name) != materials.end()){
        logMessage("Material is already loaded: %s\n", name.c_str());
        return false;
    }
    
    std::unique_ptr<FileContent> objSource = readBytesFromFile(name.c_str());
    
    if(!objSource.get()){
        logMessage("Unable to load material: %s\n", name.c_str());
        return false;
    }
    
    sp<ObjMaterial> mat = nullptr;
    char *line = strtok((char*)objSource->content, "\n"),
    str[255] = {""};
    
    v3d v;

    while(line){
        if(!line[0] || line[0] == '#' ){ line = strtok( NULL, "\n" ); continue;
        }else if( sscanf(line, "newmtl %s", str) == 1){
            mat = std::make_shared<ObjMaterial>(); mat->name = str;
            materials.insert(std::pair<std::string, sp<ObjMaterial>>(mat->name, mat));
            logMessage("New material added: %s\n", mat->name.c_str());
            logGLError();
        }else if(sscanf(line, "Ka %f %f %f", &v.x, &v.y, &v.z) == 3){
            memcpy(&mat->ambient, &v, sizeof(v3d));
                    logGLError();
        }else if(sscanf(line, "Kd %f %f %f", &v.x, &v.y, &v.z) == 3){
            memcpy(&mat->diffuse, &v, sizeof(v3d));
                    logGLError();
        }else if(sscanf(line, "Ks %f %f %f", &v.x, &v.y, &v.z) == 3){
            memcpy(&mat->specular, &v, sizeof(v3d));
                    logGLError();
        }else if(sscanf(line, "Tf %f %f %f", &v.x, &v.y, &v.z) == 3){
            memcpy(&mat->transmissionFilter, &v, sizeof(v3d));
                    logGLError();
        }else if(sscanf( line, "illum %f", &v.x ) == 1 ){
            mat->illuminationModel = (int)v.x;
                    logGLError();
        }else if(sscanf(line, "d %f", &v.x) == 1){
            mat->ambient.w = v.x;
            mat->diffuse.w = v.x;
            mat->specular.w= v.x;
            mat->dissolve   = v.x;
                    logGLError();
        }else if(sscanf(line, "Ns %f",  &v.x) == 1){
            mat->specularExponent   = v.x;
                    logGLError();
        }else if(sscanf(line, "Ni %f", &v.x) == 1){
            mat->opticalDensity = v.x;
                    logGLError();
        }else if(sscanf(line, "map_Ka %s", str) == 1){
            mat->mapAmbient = processMaterialMap(str);
                    logGLError();
        }else if(sscanf(line, "map_Kd %s", str) == 1){
            mat->mapDiffuse = processMaterialMap(str);
                    logGLError();
        }else if(sscanf(line, "map_Ks %s", str) == 1){
            mat->mapSpecular = processMaterialMap(str);
                    logGLError();
        }else if(sscanf(line, "map_Tr %s", str) == 1){
            mat->mapTranslucency = processMaterialMap(str);
                    logGLError();
        }else if( sscanf( line, "map_disp %s", str ) == 1 ||
                 sscanf( line, "map_Disp %s", str ) == 1 ||
                 sscanf( line, "disp %s"    , str ) == 1 ){
            mat->mapDisp = processMaterialMap(str);
                    logGLError();
        }else if( sscanf( line, "map_bump %s", str ) == 1 ||
                 sscanf( line, "map_Bump %s", str ) == 1 ||
                 sscanf( line, "bump %s"	, str ) == 1 ){
            mat->mapBump = processMaterialMap(str);
                    logGLError();
        }
        logGLError();
        line = strtok( NULL, "\n" );
    }
    
    logMessage("Loaded material: %s\n", mat->name.c_str());
    return true;
}

bool Materials::loadTexture(const std::string &name){
    logGLError();
    if(textures.find(name) != textures.end()){
        logMessage("Texture is already loaded: %s\n", name.c_str());
    }
    sp<Texture> texture(Texture::load(name.c_str()));
    if(texture == nullptr){
        logMessage("Unable to load texture: %s\n", name.c_str());
        return false;
    }
    logGLError();
    textures.insert(std::pair<std::string, sp<Texture>>(texture->filename, texture));
    logGLError();
    
    logMessage("Loaded texture: %s \n", texture->filename.c_str());
    return true;
}

sp<Texture> Materials::getTexture(const std::string &name){
    if(textures.find(name) != textures.end()){
        textures[name]->generateID(0, 0);
        return textures[name];
    }
    return nullptr;
}

sp<ObjMaterial> Materials::getMaterial(const std::string &name){
    if(materials.find(name) != materials.end()){
        return materials[name];
    }else{
        logMessage("cant find material %s\n", name.c_str());
        auto mat = std::make_shared<ObjMaterial>();
        mat->program = getProgram("norm_as_color");
        return mat;
    }
}

sp<ShaderProgram> Materials::getProgram(const string &name){
    if(programs.find(name) != programs.end())
        return programs[name];
    
    return nullptr;
}

sp<ShaderProgram> Materials::getProgramFoMesh(const string &name){
    if(meshShaderTable.find(name) != meshShaderTable.end()){
        logMessage("Get program for mesh name: %s %s \n", name.c_str(), meshShaderTable[name].c_str());
        return getProgram(meshShaderTable[name]);
    }
    return getProgram("normAsColor");
}

bool Materials::addMaterial(const sp<ObjMaterial> mat){
    if(materials.find(mat->name) == materials.end()){
            logMessage("Adding material: %s\n", mat->name.c_str());
        materials.insert(std::pair<string, sp<ObjMaterial>>(mat->name, mat));
        return true;
    }else
        return false;
}

bool Materials::isMaterialExist(const string &name){
    return (materials.find(name) != materials.end());
}

void Materials::loadPrograms(){
    logGLError();
    addProgram(ShaderHelper::createProgram("Normal",   "pos_norm_vertex.glsl",     "norm_as_color_fragment.glsl"   , ShaderHelper::ShaderType::Normal));
    logGLError();
    addProgram(ShaderHelper::createProgram("Grey",    "default_gray_vertex.glsl", "default_gray_fragment.glsl"   , ShaderHelper::ShaderType::Grey ));
    addProgram(ShaderHelper::createProgram("SimpleGouraud","SimpeGouraudVertex.glsl",     "SimpleGouraudFragment.glsl"      , ShaderHelper::ShaderType::SimplePhong  ));
    addProgram(ShaderHelper::createProgram("SimplePhong", "vertexPerPixel.glsl",      "fragmentPerPixel.glsl"      ,ShaderHelper::ShaderType::SimplePhong   ));
    addProgram(ShaderHelper::createProgram("wired",           "wired_vertex.glsl",        "wired_fragment.glsl"  , ShaderHelper::ShaderType::Wired ));
    
    addProgram(ShaderHelper::createProgram("lightPoint",      "light_point_vertex.glsl",  "light_point_fragment.glsl"     ));
    addProgram(ShaderHelper::createProgram("font",            "font_vertex.glsl",         "font_fragment.glsl"            ));
}

void Materials::addProgram(sp<ShaderProgram> program){
    logGLError();
    programs.insert(std::pair<string, sp<ShaderProgram>>(program->name, program));
}

bool Materials::isOMGFile(string fileName){
    return fileName.substr( fileName.find_last_of(".") + 1) == "omg" ;
}

string Materials::processMaterialMap(string name){
    logGLError();
    if(isOMGFile(name)){
        loadOMGFile(name);
        logGLError();
        return std::string();
    }else{
        loadTexture(name);
                    logGLError();
        return name;
    }

}

void Materials::loadOMGFile(string fileName){
    if(programs.find(fileName) != programs.end()){
        logMessage("OMG program is already loaded!\n");
        return;
    }
    up<FileContent> content = readTextFile(fileName);
    if(!content){
        logMessage("Unable to load OMG file %s\n", fileName.c_str());
        return;
    }
    
    char temp[52], temp_2[52];
    
    ShaderHelper::ShaderType type = ShaderHelper::ShaderHelper::General;
    char *newLine = strchr((char*)content->content, '\n');
    memcpy(temp, content->content, (newLine - (char*)content->content + 1));
    if(sscanf(temp, "type %s", temp_2) == 1){
        logMessage("Shader type : %s\n", temp_2);
        type = ShaderHelper::getTypeFromString(temp_2);
    }
    
    char vertexToken [48] = {"GL_VERTEX_SHADER"};
    char fragmentToken[48] = {"GL_FRAGMENT_SHADER"};
    char *vertexShader = strstr((char*)content->content, vertexToken);
    char *fragmentShader = strstr((char*)content->content, fragmentToken);
    logGLError();
    if((vertexShader && fragmentShader) && (fragmentShader > vertexShader)){
        vertexShader += strlen(vertexToken);
        *fragmentShader = 0;
        Shader vShader = ShaderHelper::createShader(GL_VERTEX_SHADER, vertexShader, "tempVertex");
        
        fragmentShader += strlen(fragmentToken);
        
        Shader fShader = ShaderHelper::createShader(GL_FRAGMENT_SHADER, fragmentShader, "tempFragment");
        
        sp<ShaderProgram> program =  ShaderHelper::createProgram(fileName, vShader, fShader, type);
        
        addProgram(program);
        
        if(!program->name.compare("writedepth.omg")){
            sp<ObjMaterial> shadowMaterial = std::make_shared<ObjMaterial>("shadowMaterial");
            shadowMaterial->program = program;
            addMaterial(shadowMaterial);
            logGLError();
        }
        logGLError();
    }else{
        logMessage("Omg file bad format:( \n");
        return;
    }
}

