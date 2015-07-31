#pragma once
#include "SRUtils.h"
#include "ShaderHelper.h"

struct SRShader{
    char name[MAX_CHAR];
    unsigned int type;
    unsigned int ID;

    SRShader(char *fileName, unsigned int type);
    ~SRShader();

    bool        compile(bool debug);
    void        deleteID();
};