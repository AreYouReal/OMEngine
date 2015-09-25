#pragma once

#include <memory>
#include <map>

#include "ObjMaterial.h"

class Materials{
public:
    static std::shared_ptr<Materials> instance();
    
    bool        loadMaterial(const std::string &name);
    OBJMATERIAL getMaterial (const std::string &name);
    
private:
// Constructor
    Materials();
    
// Fields
    // Static
    static std::shared_ptr<Materials> mInstance;

    // Member
    std::map<std::string, OBJMATERIAL> materials;
};