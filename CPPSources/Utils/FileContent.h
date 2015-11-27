#pragma once
#include "OMUtils.h"

struct FileContent{
    FileContent(char* contentChars, long cSize) : content(contentChars), size(cSize){}
    ~FileContent(){ if(content) delete[] content; }

    const char   *content;
    long         size;
};