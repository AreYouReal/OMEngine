//
//  obj.cpp
//  iOSSRE
//
//  Created by Alexander Kolesnikov on 31/07/2015.
//  Copyright (c) 2015 Daniel Ginsburg. All rights reserved.
//

#include "obj.h"
#include "main.h"


Obj* Obj::load(const char* fileName){
    unsigned char* objSource = readOBJFromFile(SRGraphics::getAppContext(), fileName);
    if(!objSource) return nullptr;
    
    ObjMesh* objMesh = nullptr;
    ObjTriangleList* objTriangleList = nullptr;
    
    v3d v;
    
    Obj* obj = new Obj();
    
    
    char* line = strtok((char*)objSource, "\n");
    while(line){
        unsigned char position = ( unsigned char * )line - objSource + strlen( line ) + 1;
        logMessage("%s", line);
        line = strtok( ( char * )&objSource[ position ], "\n" );
        continue;
    }
    
    if(objSource) delete [] objSource;
    
    return nullptr;
}