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
    char last = 0;
    
    
    char name[MAX_CHAR],
    usemtl[MAX_CHAR];
    
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
                ++obj->nObjMesh;
                obj->objMesh = new ObjMesh();
                objMesh = &obj->objMesh[obj->nObjMesh - 1];
                objMesh->scale[0] = objMesh->scale[1] = objMesh->scale[2] = 1.0f;
                objMesh->visible = true;
                
                if(name[0]) strcpy(objMesh->name, name);
                else if(usemtl[0]) strcpy(objMesh->name, name);
                
//                if( group[ 0 ] ) strcpy( objmesh->group, group );
                
//                objmesh->use_smooth_normals = use_smooth_normals;
                
                ++objMesh->nTrinagleList;
                objMesh->objTriangleList    = new ObjTriangleList[objMesh->nTrinagleList];
                objTriangleList = &objMesh->objTriangleList[ objMesh->nTrinagleList ];
                objTriangleList->mode = GL_TRIANGLES;
                if(useUVs) objTriangleList->useUVs = useUVs;
//                if(usemtl[0]) objTriangleList->objMaterial = obj->getMaterial(usemtl, 1);  ???
                name[0]     = 0;
                usemtl[0]   = 0;
                
                
                
            }
            
            
        }
        
        
    }
    
    if(objSource) delete [] objSource;
    
    return nullptr;
}


void ObjMesh::addVertexData(ObjTriangleList *otl, int vIndex, int uvIndex){
    unsigned short index;
    for(index = 0; index < nObjVertexData; ++index){
        if(vIndex == objVertexData[index].vertexIndex){
            if(uvIndex == -1 || uvIndex == objVertexData[index].uvIndex){
                addIndexToTriangleList(otl, index);
            }
        }
    }
    
    index = nObjVertexData;
    ++nObjVertexData;
    objVertexData = (ObjVertexData *) realloc(objVertexData, nObjVertexData * sizeof(ObjVertexData));
    objVertexData[index].vertexIndex = vIndex;
    objVertexData[index].uvIndex = uvIndex;

}

void ObjMesh::addIndexToTriangleList(ObjTriangleList *otl, int index){
    ++otl->nIndiceArray;
    otl->indiceArray = (unsigned short *) realloc(otl->indiceArray, otl->nIndiceArray * sizeof(unsigned short));
    otl->indiceArray[otl->nIndiceArray - 1] = index;
}

