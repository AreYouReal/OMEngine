#pragma once

#include "SRUtils.h"

struct Shader{
    Shader();
    ~Shader();
    
    std::string name;
    unsigned int type;
    unsigned int ID;
    
    void        deleteID();
};