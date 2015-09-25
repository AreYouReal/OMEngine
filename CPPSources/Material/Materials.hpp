#pragma once

#include <memory>
#include <map>

#include "Texture.h"
#include "ObjMaterial.h"

class ObjMaterial;

class Materials{
public:
    static std::shared_ptr<Materials> instance();
    
    bool        loadMaterial(const std::string &name);
    bool        loadTexture (const std::string &name);
    std::shared_ptr<ObjMaterial> getMaterial (const std::string &name);
    TEXTURE     getTexture  (const std::string &name);
    
private:
// Constructor
    Materials();
    
// Fields
    // Static
    static std::shared_ptr<Materials> mInstance;

    // Member
    std::map<std::string, std::shared_ptr<ObjMaterial>> materials;
    std::map<std::string, TEXTURE>     textures;
};