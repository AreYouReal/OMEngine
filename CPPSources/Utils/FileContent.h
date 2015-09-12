#pragma once
#include "SRUtils.h"

struct FileContent{
    
    FileContent(char* contentChars, long cSize){ content = contentChars; size = cSize;}
    ~FileContent(){ if(content) delete[] content;
//        logMessage("FILE CONTENT DESTRUCTOR!");
    }

    const char   *content;
    long         size;
};