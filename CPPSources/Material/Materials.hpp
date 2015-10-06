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

    bool        loadMaterial(const std::string &name);
    bool        loadTexture (const std::string &name);
    ObjMaterial *getMaterial (const std::string &name);
    sp<Texture> getTexture  (const std::string &name);
    
private:
// Fields
    
    // Member
    std::map<std::string, ObjMaterial*> materials;
    std::map<std::string, sp<Texture>>  textures;
    

};