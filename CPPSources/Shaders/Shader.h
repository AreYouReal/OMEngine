#pragma once

#include "SRUtils.h"

struct Shader{
    std::string name;
    unsigned int type;
    unsigned int ID;

    ~Shader();
    
    void        deleteID();
};