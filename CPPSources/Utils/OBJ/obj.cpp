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
    
//    
//    unsigned char position = ( unsigned char * )line - objSource + strlen( line ) + 1;
//    logMessage("%s", line);
//    line = strtok( ( char * )&objSource[ position ], "\n" );
    
    char* line = strtok((char*)objSource, "\n");
    char last;
    while(line){
        if(!line[0] || line[0] == '#'){
            // go to next line
            line[0] = 0;
            strtok(NULL, "\n");
        }else if( line[0] == 'f' && line[1] == ' '){
            bool useUVs;
            int vertexIndex[3]  = {0, 0, 0},
                normalIndex[3]  = {0, 0, 0},
                uvIndex[3]     = {0, 0, 0},
                triangle_index;
            
            if( sscanf(line, "f %d %d %d", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]) == 3){
                useUVs = false;
            }else if( sscanf(line, "f %d//%d %d//%d %d//%d", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]) == 6){
                useUVs = false;
            }else if( sscanf(line, "f %d/%d %d/%d %d/%d", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]) == 6){
                useUVs = true;
            }else{
                sscanf( line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &vertexIndex[0], &uvIndex[0],&normalIndex[0],
                       &vertexIndex[1], &uvIndex[1],&normalIndex[1],
                       &vertexIndex[2], &uvIndex[2],&normalIndex[2]);
                
                useUVs = true;
            }
            
            if( last != 'f'){
            
            }
            
            
        }
        
        
    }
    
    if(objSource) delete [] objSource;
    
    return nullptr;
}