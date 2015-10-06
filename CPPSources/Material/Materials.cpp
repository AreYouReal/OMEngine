
#include "Materials.hpp"
#include "Game.h"
#include "SRUtils.h"
#include "ShaderLibrary.h"

Materials::Materials(){
    logMessage("Materials constructor!\n");
}

Materials::~Materials(){
    for (auto it= materials.begin(); it!=materials.end(); ++it){
        ObjMaterial *mat = it->second;
        if(mat){ delete mat; mat = 0; }
    }
    materials.clear();
    logMessage("Materials destructor!\n");
}


bool Materials::loadMaterial(const std::string &name){
    logMessage("Starting loading materials! %d\n", materials.size());
    if(materials.find(name) != materials.end()){
        logMessage("Material is already loaded: %s\n", name.c_str());
        return false;
    }
    
    std::unique_ptr<FileContent> objSource = readOBJFromFile(Game::getAppContext(), name.c_str());
    
    if(!objSource.get()){
        logMessage("Unable to load material: %s\n", name.c_str());
        return false;
    }
    
    ObjMaterial *mat = NULL;
    char *line = strtok((char*)objSource->content, "\n"),
    str[255] = {""};
    
    v3d v;
    
    logMessage("Line processing! %d\n", materials.size());
    
    while(line){
        if(!line[0] || line[0] == '#' ){ line = strtok( NULL, "\n" ); continue;
        }else if( sscanf(line, "newmtl %s", str) == 1){
            mat = new ObjMaterial(); mat->name = str;
            materials.insert(std::pair<std::string, ObjMaterial*>(mat->name, mat));
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
            mat->mapAmbient = str;
            loadTexture(str);
        }else if(sscanf(line, "map_Kd %s", str) == 1){
            mat->mapDiffuse = str;
            loadTexture(str);
        }else if(sscanf(line, "map_Ks %s", str) == 1){
            mat->mapSpecular = str;
            loadTexture(str);
        }else if(sscanf(line, "map_Tr %s", str) == 1){
            mat->mapTranslucency = str;
            loadTexture(str);
        }else if( sscanf( line, "map_disp %s", str ) == 1 ||
                 sscanf( line, "map_Disp %s", str ) == 1 ||
                 sscanf( line, "disp %s"    , str ) == 1 ){
            mat->mapDisp = str;
            loadTexture(str);
        }else if( sscanf( line, "map_bump %s", str ) == 1 ||
                 sscanf( line, "map_Bump %s", str ) == 1 ||
                 sscanf( line, "bump %s"	, str ) == 1 ){
            mat->mapBump = str;
            loadTexture(str);
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
    sp<Texture> texture(Texture::load(Game::getAppContext(), name.c_str(), TextureSource::PNG));
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

ObjMaterial *Materials::getMaterial(const std::string &name){
    if(materials.find(name) != materials.end()){
        return materials[name];
    }else{
        logMessage("cant find material %s\n", name.c_str());
        ObjMaterial* mat = new ObjMaterial();
        mat->program = ShaderLibrary::instance()->getProgram("norm_as_color");
        return mat;
    }
}

