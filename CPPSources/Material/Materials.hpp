#pragma once

#include <memory>
#include <map>

#include "Texture.h"
#include "ObjMaterial.h"

class ObjMaterial;

class Materials{
public:
    
    ~Materials();
    
    static Materials *instance();
    
    bool        loadMaterial(const std::string &name);
    bool        loadTexture (const std::string &name);
    ObjMaterial *getMaterial (const std::string &name);
    sp<Texture> getTexture  (const std::string &name);
    
private:
// Constructor
    Materials();
    
// Fields
    // Static
    static Materials *mInstance;

    // Member
    std::map<std::string, ObjMaterial*> materials;
    std::map<std::string, sp<Texture>>  textures;
};