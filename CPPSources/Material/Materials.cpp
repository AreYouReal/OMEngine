
#include "Materials.hpp"
#include "OMUtils.h"
#include "ShaderHelper.h"

static std::map<string, string>  meshShaderTable{
    // treemomo.obj meshes
    std::pair<string, string>("background", "defaultPerVertex"),
    std::pair<string, string>("baloon", "defaultPerVertex"),
    std::pair<string, string>("grass_ground", "defaultPerVertex"),
    std::pair<string, string>("momo", "defaultPerVertex"),
    std::pair<string, string>("tree", "defaultPerVertex"),
    std::pair<string, string>("leaf", "defaultPerVertex"),
    // Scene.obj meshes
    std::pair<string, string>("floor", "generalLightingProjector.omg"),
    std::pair<string, string>("sphere2_sphere.002", "generalLightingProjector.omg"),
    std::pair<string, string>("sphere1_sphere.001", "generalLightingProjector.omg"),
    std::pair<string, string>("sphere3_sphere", "generalLightingProjector.omg")
};

Materials::Materials(){
    loadPrograms();
}

Materials::~Materials(){
    materials.clear();
}

bool Materials::loadMaterial(const std::string &name){
    logGLError();
//    logMessage("Starting loading materials! %d\n", materials.size());
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
    
//    logMessage("Line processing! %d\n", materials.size());
    
    while(line){
        if(!line[0] || line[0] == '#' ){ line = strtok( NULL, "\n" ); continue;
        }else if( sscanf(line, "newmtl %s", str) == 1){
            mat = std::make_shared<ObjMaterial>(); mat->name = str;
            materials.insert(std::pair<std::string, sp<ObjMaterial>>(mat->name, mat));
        }else if(sscanf(line, "Ka %f %f %f", &v.x, &v.y, &v.z) == 3){
            memcpy(&mat->ambient, &v, sizeof(v3d));
        }else if(sscanf(line, "Kd %f %f %f", &v.x, &v.y, &v.z) == 3){
            memcpy(&mat->diffuse, &v, sizeof(v3d));
        }else if(sscanf(line, "Ks %f %f %f", &v.x, &v.y, &v.z) == 3){
            memcpy(&mat->specular, &v, sizeof(v3d));
        }else if(sscanf(line, "Tf %f %f %f", &v.x, &v.y, &v.z) == 3){
            memcpy(&mat->transmissionFilter, &v, sizeof(v3d));
        }else if(sscanf( line, "illum %f", &v.x ) == 1 ){
            mat->illuminationModel = (int)v.x;
        }else if(sscanf(line, "d %f", &v.x) == 1){
            mat->ambient.w = v.x;
            mat->diffuse.w = v.x;
            mat->specular.w= v.x;
            mat->dissolve   = v.x;
        }else if(sscanf(line, "Ns %f",  &v.x) == 1){
            mat->specularExponent   = v.x;
        }else if(sscanf(line, "Ni %f", &v.x) == 1){
            mat->opticalDensity = v.x;
        }else if(sscanf(line, "map_Ka %s", str) == 1){
            mat->mapAmbient = processMaterialMap(str);
        }else if(sscanf(line, "map_Kd %s", str) == 1){
            mat->mapDiffuse = processMaterialMap(str);
        }else if(sscanf(line, "map_Ks %s", str) == 1){
            mat->mapSpecular = processMaterialMap(str);
        }else if(sscanf(line, "map_Tr %s", str) == 1){
            mat->mapTranslucency = processMaterialMap(str);
        }else if( sscanf( line, "map_disp %s", str ) == 1 ||
                 sscanf( line, "map_Disp %s", str ) == 1 ||
                 sscanf( line, "disp %s"    , str ) == 1 ){
            mat->mapDisp = processMaterialMap(str);
        }else if( sscanf( line, "map_bump %s", str ) == 1 ||
                 sscanf( line, "map_Bump %s", str ) == 1 ||
                 sscanf( line, "bump %s"	, str ) == 1 ){
            mat->mapBump = processMaterialMap(str);
        }
        line = strtok( NULL, "\n" );
    }
    
    logMessage("%s", objSource->content);
    return true;
}

bool Materials::loadTexture(const std::string &name){
    if(textures.find(name) != textures.end()){
        logMessage("Texture is already loaded: %s\n", name.c_str());
    }
    sp<Texture> texture(Texture::load(name.c_str()));
    if(texture == nullptr){
        logMessage("Unable to load texture: %s\n", name.c_str());
        return false;
    }
    textures.insert(std::pair<std::string, sp<Texture>>(texture->filename, texture));
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
    addProgram(ShaderHelper::createProgram("normAsColor",   "pos_norm_vertex.glsl",     "norm_as_color_fragment.glsl"   , ShaderHelper::ShaderType::Normal));
    logGLError();
    addProgram(ShaderHelper::createProgram("defaultGrey",    "default_gray_vertex.glsl", "default_gray_fragment.glsl"   , ShaderHelper::ShaderType::Grey ));
    addProgram(ShaderHelper::createProgram("defaultPerVertex","vertexPerVertex.glsl",     "fragmentPerVertex.glsl"      , ShaderHelper::ShaderType::DefaultPerVertex  ));
    addProgram(ShaderHelper::createProgram("defaultPerPixel", "vertexPerPixel.glsl",      "fragmentPerPixel.glsl"      ,ShaderHelper::ShaderType::DefaultPerPixel   ));
    addProgram(ShaderHelper::createProgram("wired",           "wired_vertex.glsl",        "wired_fragment.glsl"  , ShaderHelper::ShaderType::Wired         ));
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
    if(isOMGFile(name)){
        loadOMGFile(name);
        return std::string();
    }else{
        loadTexture(name);
        return name;
    }
}

void Materials::loadOMGFile(string fileName){
    up<FileContent> content = readTextFile(fileName);
    if(!content){
        logMessage("Unable to load OMG file %s", fileName.c_str());
        return;
    }
    
    char vertexToken [48] = {"GL_VERTEX_SHADER"};
    char fragmentToken[48] = {"GL_FRAGMENT_SHADER"};
    char *vertexShader = strstr((char*)content->content, vertexToken);
    char *fragmentShader = strstr((char*)content->content, fragmentToken);
    
    if((vertexShader && fragmentShader) && (fragmentShader > vertexShader)){
        vertexShader += strlen(vertexToken);
        *fragmentShader = 0;
        Shader vShader = ShaderHelper::createShader(GL_VERTEX_SHADER, vertexShader, "tempVertex");
        
        fragmentShader += strlen(fragmentToken);
        
        Shader fShader = ShaderHelper::createShader(GL_FRAGMENT_SHADER, fragmentShader, "tempFragment");
        
        sp<ShaderProgram> program =  ShaderHelper::createProgram(fileName, vShader, fShader);
        
        addProgram(program);
        
        if(!program->name.compare("writedepth.omg")){
            sp<ObjMaterial> shadowMaterial = std::make_shared<ObjMaterial>("shadowMaterial");
            shadowMaterial->program = program;
            addMaterial(shadowMaterial);
        }
        
    }else{
        logMessage("Omg file bad format:( ");
        return;
    }
}

