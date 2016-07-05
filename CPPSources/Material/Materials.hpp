#pragma once

#include <memory>
#include <map>

#include "Singleton.hpp"
#include "Texture.h"

class ObjMaterial;
class ShaderProgram;

class Materials : public Singleton<Materials>{
public:

    Materials();
    Materials(const Materials& rhs) = delete;
    Materials& operator=(const Materials& rhs) = delete;
    ~Materials();

    bool                loadMaterial(const string &name);
    bool                loadTexture (const string &name, bool generateID);
    sp<ObjMaterial>     getMaterial (const string &name);
    sp<Texture>         getTexture  (const string &name);
    sp<ShaderProgram>   getProgram  (const string &name);
    sp<ShaderProgram>   getProgramFoMesh(const string &name);
    
    bool                isMaterialExist(const string &name);
    bool                addMaterial(const sp<ObjMaterial> mat);
    
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