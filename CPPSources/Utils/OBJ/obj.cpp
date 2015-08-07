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
    
    
    char name[MAX_CHAR], str[MAX_CHAR],usemtl[MAX_CHAR], group[MAX_CHAR];
    bool useSmoothNormals;
    
    while(line){
        if(!line[0] || line[0] == '#'){
            // go to next object line
            last = line[0];
            strtok(NULL, "\n");
            continue;
        }else if( line[0] == 'f' && line[1] == ' '){
            bool useUVs;
            int vertexIndex[3]  = {0, 0, 0},
                normalIndex[3]  = {0, 0, 0},
                uvIndex[3]     = {0, 0, 0},
                triangleIndex;
            
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
            logMessage("f ->   %d, %d, %d, %d, %d ,%d, %d, %d ,%d \n", vertexIndex[0], vertexIndex[1], vertexIndex[2],
                       uvIndex[0], uvIndex[1], uvIndex[2],
                       normalIndex[0], normalIndex[1], normalIndex[2]);
            
            if( last != 'f'){
                logMessage("\nlast != f: \n");
                
                
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
            
                
            --vertexIndex[0];--vertexIndex[1];--vertexIndex[2];
            --uvIndex[0];--uvIndex[1];--uvIndex[2];
            
            objMesh->addVertexData(objTriangleList, vertexIndex[0], uvIndex[0]);
            objMesh->addVertexData(objTriangleList, vertexIndex[1], uvIndex[1]);
            objMesh->addVertexData(objTriangleList, vertexIndex[2], uvIndex[2]);
            
            triangleIndex = objTriangleList->nObjTriangleIndex;
            ++objTriangleList->nObjTriangleIndex;
            objTriangleList->objTriangleIndex = (ObjTriangleIndex *) realloc( objTriangleList->objTriangleIndex,
                                                                                                                                                           objTriangleList->nObjTriangleIndex * sizeof(ObjTriangleIndex));
                
            for(int i = 0; i < 3; ++i){
                objTriangleList->objTriangleIndex[triangleIndex].vertexIndex[i] = vertexIndex[i];
                objTriangleList->objTriangleIndex[triangleIndex].uvIndex[i]     = uvIndex[i];
            }
            
        }else if(sscanf(line, "v %f %f %f", &v[0], &v[1], &v[2]) == 3){
            // Vertex
            logMessage("v  ->  Vertex: %f, %f, %f \n", v[0], v[1], v[2] );
            ++obj->nIndexedVertex;
            obj->indexedVertex = (v3d *) realloc(obj->indexedVertex, obj->nIndexedVertex * sizeof(v3d));
            memcpy(&obj->indexedVertex[obj->nIndexedVertex - 1], &v, sizeof(v3d));
            // Normal
            obj->indexedNormal = (v3d *) realloc(obj->indexedNormal, obj->nIndexedVertex * sizeof(v3d));
            obj->indexedFNormal = (v3d *) realloc(obj->indexedFNormal, obj->nIndexedVertex * sizeof(v3d));
            memset(&obj->indexedNormal, 0, sizeof(v3d));
            memset(&obj->indexedFNormal, 0, sizeof(v3d));
            // Tangent
            obj->indexedTangent = (v3d *) realloc(obj->indexedTangent, obj->nIndexedVertex * sizeof(v3d));
            memset(&obj->indexedTangent, 0, sizeof(v3d));
        } else if(sscanf(line, "vn %f %f %f", &v[0], &v[1], &v[2]) == 3){
            logMessage(" vn   -> Drop the normals: %f, %f, %f \n", v[0], v[1], v[2] );
            // go to next object line
//            last = line[0];
//            strtok(NULL, "\n");
        } else if(sscanf(line, "vn %f %f", &v[0], &v[1]) == 2){
            ++obj->nIndexedUV;
            obj->indexedUV = (v3d *) realloc(obj->indexedUV, obj->nIndexedUV * sizeof(v3d));
            v[1] = 1.0f - v[1];
            memcpy(&obj->indexedUV[obj->nIndexedUV - 1], &v, sizeof(v3d));
        }else if(line[0] == 'v' && line[1] == 'n' ){
//            last = line[0];
//            strtok(NULL, "\n");
//            continue;
        }else if(sscanf(line, "usemtl %s", str) == 1){ strcpy(usemtl, str);
        }else if(sscanf(line, "o %s", str) == 1){ strcpy(name, str);
        }else if(sscanf(line, "g %s", str) == 1){ strcpy(group, str);
        }else if(sscanf(line, "s %s", str) == 1){
            useSmoothNormals = true;
            if(strstr( str, "off") || strstr(str, "0") ) useSmoothNormals = false;
        }else if(sscanf(line, "mtllib %s", str) == 1){
            unsigned char position = (unsigned char *) line - objSource + strlen(line) + 1;
            // OBJ_LOAD_MTL(obj, );
            line = strtok((char *) &objSource[position], "\n");
            continue;
        }
        
        last = line[0];
        line = strtok(NULL, "\n");
    }
    
    if(objSource) delete [] objSource;
    
    return nullptr;
}



#pragma mark Helpers
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

