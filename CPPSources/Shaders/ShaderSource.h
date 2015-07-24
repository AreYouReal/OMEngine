#pragma once

#include "SRUtils.h"

struct ShaderSource{
    
    char* source = nullptr;
    int size   = -1;
    
    ~ShaderSource(){
        if(source) delete[] source;
    }
};
