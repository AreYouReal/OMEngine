#pragma once

#include "OMUtils.h"
#include "Shortcuts.h"

struct Shader{
    Shader();
    ~Shader();
    
    std::string name;
    unsigned int type;
    unsigned int ID;
    
    void        deleteID();
};