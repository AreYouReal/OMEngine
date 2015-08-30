//
//  obj.cpp
//  iOSSRE
//
//  Created by Alexander Kolesnikov on 31/07/2015.
//  Copyright (c) 2015 Daniel Ginsburg. All rights reserved.
//

#include "obj.h"
#include "main.h"


Obj::~Obj(){
    logMessage("Object destructor");
}


ObjMesh::~ObjMesh(){
    if(material){
        delete material;
        material = 0;
    }
    logMessage("ObjMEsh destructor");
}

ObjTriangleList::~ObjTriangleList(){
    if(tIndex){
        free(tIndex);
        tIndex = 0;
    }
    if(indices){
        free(indices);
        indices = 0;
    }
    logMessage("ObjTriangleList destructor");
}

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
                uvIndex[3]      = {0, 0, 0},
                triangleIndex;
            
            Obj::readIndices(line, vertexIndex, normalIndex, uvIndex, useUVs);

            
            if( last != 'f'){
//                logMessage("\nlast != f: \n");
                obj->meshes.push_back(ObjMesh());
                objMesh = &obj->meshes.back();
                objMesh->visible = true;
                
                if(name[0]) strcpy(objMesh->name, name);
                else if(usemtl[0]) strcpy(objMesh->name, name);
                
//                if( group[ 0 ] ) strcpy( objmesh->group, group );
                
//                objmesh->use_smooth_normals = use_smooth_normals;
                objMesh->tLists.push_back(ObjTriangleList());
                objTriangleList = &objMesh->tLists.back();
                
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
            
            triangleIndex = objTriangleList->nTIndex;
            ++objTriangleList->nTIndex;
            objTriangleList->tIndex = (ObjTriangleIndex *) realloc( objTriangleList->tIndex,
                                                                                                                                                           objTriangleList->nTIndex * sizeof(ObjTriangleIndex));
                
            for(int i = 0; i < 3; ++i){
                objTriangleList->tIndex[triangleIndex].vertexIndex[i] = vertexIndex[i];
                objTriangleList->tIndex[triangleIndex].uvIndex[i]     = uvIndex[i];
            }
        
        }else if(sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z) == 3){
            // Vertex
//            logMessage("v  ->  Vertex: %f, %f, %f \n", v.x, v.y, v.z );
            obj->vertices.push_back(v);
            // Normal
            obj->normals.push_back(v3d(0.0f));
            obj->faceNormals.push_back(v3d(0.0f));
            // Tangent
            obj->tangents.push_back(v3d(0.0f));
        } else if(sscanf(line, "vn %f %f %f", &v.x, &v.y, &v.z) == 3){
//            logMessage(" vn   -> Drop the normals: %f, %f, %f \n", v.x, v.y, v.z );
            // go to next object line
        } else if(sscanf(line, "vs %f %f", &v.x, &v.y) == 2){
            v.y = 1.0f - v.y;
            obj->UVs.push_back(v);
        }else if(line[0] == 'v' && line[1] == 'n' ){
            last = line[0];
        }else if(sscanf(line, "usemtl %s", str) == 1){ strcpy(usemtl, str);
        }else if(sscanf(line, "o %s", str) == 1){ strcpy(name, str);
        }else if(sscanf(line, "g %s", str) == 1){ strcpy(group, str);
        }else if(sscanf(line, "s %s", str) == 1){
            useSmoothNormals = true;
            if(strstr( str, "off") || strstr(str, "0") ) useSmoothNormals = false;
        }else if(sscanf(line, "mtllib %s", str) == 1){
            unsigned char position = (unsigned char *) line - objSource + strlen(line) + 1;
            obj->loadMaterial(str);
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
        for(unsigned int i = 0; i < obj->meshes.size(); ++i){
            ObjMesh *mesh = &obj->meshes[i];
            
            // Accumulate normals and tangent
            for(unsigned int j = 0; j < mesh->tLists.size(); ++j){
                ObjTriangleList *list = &mesh->tLists[j];
                for(unsigned int k = 0; k < list->nTIndex; ++k){
                    v3d v1, v2, normal;
                    v1 = obj->vertices[list->tIndex[k].vertexIndex[0]] - obj->vertices[list->tIndex[k].vertexIndex[1]];
                    v2 = obj->vertices[list->tIndex[k].vertexIndex[0]] - obj->vertices[list->tIndex[k].vertexIndex[2]];
                    
                    normal = v3d::normalize(v3d::cross(v1, v2));
                    for(unsigned short e = 0; e < 3; ++e) {
                        memcpy(&obj->faceNormals[list->tIndex[k].vertexIndex[e]], normal.pointer(), sizeof(v3d));
                    }
                    for(unsigned short e = 0; e < 3; ++e){
                        obj->normals[list->tIndex[k].vertexIndex[e]] = obj->normals[list->tIndex[k].vertexIndex[e]] + normal;
                        
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
        for(unsigned int i = 0; i < obj->meshes.size(); ++i){
            for(unsigned int j = 0; j < obj->meshes[i].nObjVertexData; ++j){
                index = obj->meshes[i].objVertexData[j].vIndex;
                
                obj->normals[index] = v3d::normalize(obj->normals[index]);
                if(obj->meshes[i].objVertexData[j].uvIndex != -1){
                    obj->tangents[index] = v3d::normalize(obj->tangents[index]);
                }
                
            }
        }
    }
    
    return obj;
}



#pragma mark Helpers
void Obj::readIndices(const char* line, int v[], int n[], int uv[], bool &useUVs){
    if( sscanf(line, "f %d %d %d", &v[0], &v[1], &v[2]) == 3){
        useUVs = false;
    }else if( sscanf(line, "f %d//%d %d//%d %d//%d", &v[0], &n[0], &v[1], &n[1], &v[2], &n[2]) == 6){
        useUVs = false;
    }else if( sscanf(line, "f %d/%d %d/%d %d/%d", &v[0], &uv[0], &v[1], &uv[1], &v[2], &uv[2]) == 6){
        useUVs = true;
    }else{
        sscanf( line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v[0], &uv[0], &n[0],
               &v[1], &uv[1], &n[1],
               &v[2], &uv[2], &n[2]);
        
        useUVs = true;
    }
}


void ObjMesh::addVertexData(ObjTriangleList *otl, int vIndex, int uvIndex){
    unsigned short index;
    for(index = 0; index < nObjVertexData; ++index){
        if(vIndex == objVertexData[index].vIndex){
            if(uvIndex == -1 || uvIndex == objVertexData[index].uvIndex){
                addIndexToTriangleList(otl, index);
                return;
            }
        }
    }
    
    index = nObjVertexData;
    ++nObjVertexData;
    objVertexData = (ObjVertexData *) realloc(objVertexData, nObjVertexData * sizeof(ObjVertexData));
    objVertexData[index].vIndex = vIndex;
    objVertexData[index].uvIndex = uvIndex;
    
    addIndexToTriangleList(otl, index);
}

void ObjMesh::addIndexToTriangleList(ObjTriangleList *otl, int index){
    ++otl->nIndices;
    otl->indices = (unsigned short *) realloc(otl->indices, otl->nIndices * sizeof(unsigned short));
    otl->indices[otl->nIndices - 1] = index;
}


#pragma mark Material loading
void Obj::loadMaterial(const char *filename){
    unsigned char* objSource = readOBJFromFile(SRGraphics::getAppContext(), filename);
    
    if(!objSource) return;
    
    ObjMaterial *mat = NULL;
    
    char *line = strtok((char*)objSource, "\n"),
                 str[MAX_CHAR] = {""};
    
    v3d v;
    
    while(line){
        if(!line[0] || line[0] == '#' ){ line = strtok( NULL, "\n" ); continue;
        }else if( sscanf(line, "newmtl %s", str) == 1){
            logMessage("newmtl line %s", str);
            ++nMaterials;
            materials = (ObjMaterial*) realloc(materials, nMaterials * sizeof(ObjMaterial));
            mat = &materials[nMaterials - 1];
            memset(mat, 0, sizeof(ObjMaterial));
            strcpy(mat->name, str);
        }else if(sscanf(line, "Ka %f %f %f", &v.x, &v.y, &v.z) == 3){
            memcpy(&mat->ambient, &v, sizeof(v3d));
        }else if(sscanf(line, "Kd %f %f %f", &v.x, &v.y, &v.z) == 3){
            memcpy(&mat->diffuse, &v, sizeof(v3d));
        }else if(sscanf(line, "Ks %f %f %f", &v.x, &v.y, &v.z) == 3){
            memcpy(&mat->specular, &v, sizeof(v3d));
        }else if(sscanf(line, "Tf %f %f %f", &v.x, &v.y, &v.z) == 3){
            memcpy(&mat->transmissionFilter, &v, sizeof(v3d));
        }else if(sscanf( line, "illum %f", &v.x ) == 1 ){
            mat->illuminationModel = (int)v.x;
        }else if(sscanf(line, "d %f", &v.x) == 1){
            mat->ambient.w = v.x;
            mat->diffuse.w = v.x;
            mat->specular.w= v.x;
            mat->dissolve   = v.x;
        }else if(sscanf(line, "Ns %f",  &v.x) == 1){
            mat->specularExponent   = v.x;
        }else if(sscanf(line, "Ni %f", &v.x) == 1){
            mat->opticalDensity = v.x;
        }else if(sscanf(line, "map_Ka %s", str) == 1){
//            mat->mapAmbient
        }else if(sscanf(line, "map_Kd %s", str) == 1){
            addTexture(str);
        }else if(sscanf(line, "map_Ks %s", str) == 1){
//            mat->mapSpecular
        }else if(sscanf(line, "map_Tr %s", str) == 1){
//            mat->mapTranslucency
        }else if( sscanf( line, "map_disp %s", str ) == 1 ||
                 sscanf( line, "map_Disp %s", str ) == 1 ||
                 sscanf( line, "disp %s"    , str ) == 1 ){
//            mat->mapDisp
        }else if( sscanf( line, "map_bump %s", str ) == 1 ||
                 sscanf( line, "map_Bump %s", str ) == 1 ||
                 sscanf( line, "bump %s"	, str ) == 1 ){
//            mat->mapBump
        }
        
        
        line = strtok( NULL, "\n" );
    }
    
    logMessage("%s", objSource);
    
    if(objSource) delete[] objSource;
}


void Obj::addTexture(const char *filename){
    if(getTextureIndex(filename) < 0){
        ++nTextures;
        textures = (Texture **) realloc(textures, sizeof(Texture) * nTextures);
        textures[nTextures - 1] = new Texture(SRGraphics::getAppContext(), filename);
    }
}

int Obj::getTextureIndex(const char *filename){
    for(unsigned int i = 0; i < nTextures; ++i){
        if( !strcmp(textures[i]->filename, filename) ) return i;
    }
    return -1;
}
