#pragma once

#include <memory>
#include <map>

#include "Singleton.hpp"
#include "Texture.h"
#include "ObjMaterial.h"

class ObjMaterial;

class Materials : public Singleton<Materials>{
public:

    Materials();
    Materials(const Materials& rhs) = delete;
    Materials& operator=(const Materials& rhs) = delete;
    ~Materials();

    bool                loadMaterial(const string &name);
    bool                loadTexture (const string &name);
    sp<ObjMaterial>     getMaterial (const string &name);
    sp<Texture>         getTexture  (const string &name);
    sp<ShaderProgram>   getProgram  (const string &name);
    
private:
// Fields
    
    // Member
    std::map<string, sp<ObjMaterial>>  materials;
    std::map<string, sp<Texture>>      textures;
    std::map<string, sp<ShaderProgram>>programs;

// Helpers
    void loadPrograms();
    void addProgram(sp<ShaderProgram> program);
    
    
    string processMaterialMap(string name);
    bool isOMGFile(string fileName);
    void loadOMGFile(string fileName);
};