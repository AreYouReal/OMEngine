#pragma once
#include "OMUtils.h"

struct FileContent{
    FileContent(unsigned char* contentChars,const long cSize, const std::string name) : content(contentChars), size(cSize){}
    ~FileContent(){ if(content) delete[] content; }

    std::string   name;
    unsigned char *content;
    unsigned int size;
    unsigned int position;
    
};