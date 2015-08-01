#pragma once

#include "SRUtils.h"

struct Shader{
    char name[MAX_CHAR];
    unsigned int type;
    unsigned int ID;

    ~Shader();
    
    void        deleteID();
};