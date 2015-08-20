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
//            logMessage("f ->   %d, %d, %d, %d, %d ,%d, %d, %d ,%d \n", vertexIndex[0], vertexIndex[1], vertexIndex[2],
//                       uvIndex[0], uvIndex[1], uvIndex[2],
//                       normalIndex[0], normalIndex[1], normalIndex[2]);
            
            if( last != 'f'){
//                logMessage("\nlast != f: \n");
                
                
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
                objTriangleList = &objMesh->objTriangleList[ objMesh->nTrinagleList - 1 ];
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
//            logMessage("v  ->  Vertex: %f, %f, %f \n", v[0], v[1], v[2] );
            ++obj->nIndexedVertex;
            obj->indexedVertex = (v3d *) realloc(obj->indexedVertex, obj->nIndexedVertex * sizeof(v3d));
            memcpy(&obj->indexedVertex[obj->nIndexedVertex - 1], &v, sizeof(v3d));
            // Normal
            obj->indexedNormal = (v3d *) realloc(obj->indexedNormal, obj->nIndexedVertex * sizeof(v3d));
            obj->indexedFNormal = (v3d *) realloc(obj->indexedFNormal, obj->nIndexedVertex * sizeof(v3d));
//            memset(&obj->indexedNormal, v3d(), sizeof(v3d));
//            memset(&obj->indexedFNormal, v3d(), sizeof(v3d));
            // Tangent
            obj->indexedTangent = (v3d *) realloc(obj->indexedTangent, obj->nIndexedVertex * sizeof(v3d));
//            memset(&obj->indexedTangent, 0, sizeof(v3d));
        } else if(sscanf(line, "vn %f %f %f", &v[0], &v[1], &v[2]) == 3){
//            logMessage(" vn   -> Drop the normals: %f, %f, %f \n", v[0], v[1], v[2] );
            // go to next object line
        } else if(sscanf(line, "vs %f %f", &v[0], &v[1]) == 2){
            ++obj->nIndexedUV;
            obj->indexedUV = (v3d *) realloc(obj->indexedUV, obj->nIndexedUV * sizeof(v3d));
            v[1] = 1.0f - v[1];
            memcpy(&obj->indexedUV[obj->nIndexedUV - 1], &v, sizeof(v3d));
        }else if(line[0] == 'v' && line[1] == 'n' ){
//            last = line[0];
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
    
    // Build the normals and tangent
    {
        for(unsigned int i = 0; i < obj->nObjMesh; ++i){
            ObjMesh *mesh = &obj->objMesh[i];
            
            // Accumulate normals and tangent
            for(unsigned int j = 0; j < mesh->nTrinagleList; ++j){
                ObjTriangleList *list = &mesh->objTriangleList[j];
                for(unsigned int k = 0; k < list->nObjTriangleIndex; ++k){
                    v3d v1, v2, normal;
                    v1 = obj->indexedVertex[list->objTriangleIndex[k].vertexIndex[0]] - obj->indexedVertex[list->objTriangleIndex[k].vertexIndex[1]];
                    v2 = obj->indexedVertex[list->objTriangleIndex[k].vertexIndex[0]] - obj->indexedVertex[list->objTriangleIndex[k].vertexIndex[2]];
                    
                    normal = v3d::normalize(v3d::cross(v1, v2));
                    for(unsigned short e = 0; e < 3; ++e) {
                        memcpy(&obj->indexedFNormal[list->objTriangleIndex[k].vertexIndex[e]], &normal, sizeof(v3d));
                    }
                    
                    for(unsigned short e = 0; e < 3; ++e){
                        obj->indexedNormal[list->objTriangleIndex[k].vertexIndex[e]] = obj->indexedNormal[list->objTriangleIndex[k].vertexIndex[e]] + normal;
                    }
                    
                    if(list->useUVs){
//                        vec3 tangent;
//                        
//                        vec2 uv1, uv2;
//                        
//                        float c;
//                        
//                        vec2_diff( &uv1,
//                                  &obj->indexed_uv[ objtrianglelist->objtriangleindex[ k ].uv_index[ 2 ] ],
//                                  &obj->indexed_uv[ objtrianglelist->objtriangleindex[ k ].uv_index[ 0 ] ] );
//                        
//                        vec2_diff( &uv2,
//                                  &obj->indexed_uv[ objtrianglelist->objtriangleindex[ k ].uv_index[ 1 ] ],
//                                  &obj->indexed_uv[ objtrianglelist->objtriangleindex[ k ].uv_index[ 0 ] ] );
//                        
//                        
//                        c = 1.0f / ( uv1.x * uv2.y - uv2.x * uv1.y );
//                        
//                        tangent.x = ( v1.x * uv2.y + v2.x * uv1.y ) * c;
//                        tangent.y = ( v1.y * uv2.y + v2.y * uv1.y ) * c;
//                        tangent.z = ( v1.z * uv2.y + v2.z * uv1.y ) * c;
//                        
//                        
//                        vec3_add( &obj->indexed_tangent[ objtrianglelist->objtriangleindex[ k ].vertex_index[ 0 ] ],
//                                 &obj->indexed_tangent[ objtrianglelist->objtriangleindex[ k ].vertex_index[ 0 ] ],
//                                 &tangent );
//                        
//                        vec3_add( &obj->indexed_tangent[ objtrianglelist->objtriangleindex[ k ].vertex_index[ 1 ] ], 
//                                 &obj->indexed_tangent[ objtrianglelist->objtriangleindex[ k ].vertex_index[ 1 ] ],
//                                 &tangent );
//                        
//                        vec3_add( &obj->indexed_tangent[ objtrianglelist->objtriangleindex[ k ].vertex_index[ 2 ] ], 
//                                 &obj->indexed_tangent[ objtrianglelist->objtriangleindex[ k ].vertex_index[ 2 ] ],
//                                 &tangent );
                    }
  
                    
                }
            }
        }
        
        unsigned int index;
        for(unsigned int i = 0; i < obj->nObjMesh; ++i){
            for(unsigned int j = 0; j < obj->objMesh[i].nObjVertexData; ++j){
                index = obj->objMesh[i].objVertexData[j].vertexIndex;
                
                obj->indexedNormal[index] = v3d::normalize(obj->indexedNormal[index]);
                if(obj->objMesh[i].objVertexData[j].uvIndex != -1){
                    obj->indexedTangent[index] = v3d::normalize(obj->indexedTangent[index]);
                }
                
            }
        }
    }
    
    return obj;
}



#pragma mark Helpers
void ObjMesh::addVertexData(ObjTriangleList *otl, int vIndex, int uvIndex){
    unsigned short index;
    for(index = 0; index < nObjVertexData; ++index){
        if(vIndex == objVertexData[index].vertexIndex){
            if(uvIndex == -1 || uvIndex == objVertexData[index].uvIndex){
                addIndexToTriangleList(otl, index);
                return;
            }
        }
    }
    
    index = nObjVertexData;
    ++nObjVertexData;
    objVertexData = (ObjVertexData *) realloc(objVertexData, nObjVertexData * sizeof(ObjVertexData));
    objVertexData[index].vertexIndex = vIndex;
    objVertexData[index].uvIndex = uvIndex;
    
    addIndexToTriangleList(otl, index);
}

void ObjMesh::addIndexToTriangleList(ObjTriangleList *otl, int index){
    ++otl->nIndiceArray;
    otl->indiceArray = (unsigned short *) realloc(otl->indiceArray, otl->nIndiceArray * sizeof(unsigned short));
    otl->indiceArray[otl->nIndiceArray - 1] = index;
}

