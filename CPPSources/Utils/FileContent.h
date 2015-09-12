#pragma once
#include "SRUtils.h"

struct FileContent{
    
    FileContent(char* contentChars, unsigned int cSize){ content = contentChars; size = cSize;}
    ~FileContent(){ if(content) delete[] content;
//        logMessage("FILE CONTENT DESTRUCTOR!");
    }

    const char   *content;
    long         size;
};