//
//  obj.cpp
//  iOSSRE
//
//  Created by Alexander Kolesnikov on 31/07/2015.
//  Copyright (c) 2015 Daniel Ginsburg. All rights reserved.
//

#include "obj.h"
#include "main.h"

Obj::Obj(const char* fileName){
    clock_t begin = std::clock();
    
    
    unsigned char* objSource = readOBJFromFile(SRGraphics::getAppContext(), fileName);
#pragma warning throw exception here
    if(!objSource) return;
    
    ObjMesh* currentMesh = nullptr;
    ObjTriangleList* currentTList = nullptr;
    
    v3d v;

    char* line = strtok((char*)objSource, "\n");
    char last = 0;
    
    unsigned short buffSize = 255;
    char name[buffSize], str[buffSize],usemtl[buffSize], group[buffSize];
    bool useSmoothNormals;
    
    while(line){
        if(!line[0] || line[0] == '#'){
            // go to next object line
        }else if( line[0] == 'f' && line[1] == ' '){    // Read face indices...
            bool useUVs; int vertexIndex[3]  = {0, 0, 0}, normalIndex[3] = {0, 0, 0}, uvIndex[3] = {0, 0, 0};
            Obj::readIndices(line, vertexIndex, normalIndex, uvIndex, useUVs);
            if( last != 'f') addMesh(&currentMesh, &currentTList, name, usemtl, useUVs);
            --vertexIndex[0];--vertexIndex[1];--vertexIndex[2]; --uvIndex[0];--uvIndex[1];--uvIndex[2];             // Why? Because vertexIndex in obj file starts from 1! We count from 0...
            for(unsigned short i = 0; i < 3; ++i) currentMesh->addVertexData(currentTList, vertexIndex[i], uvIndex[i]);
            currentTList->tIndices.push_back(ObjTriangleIndex());
            for(int i = 0; i < 3; ++i){
                currentTList->tIndices.back().vertexIndex[i] = vertexIndex[i];
                currentTList->tIndices.back().uvIndex[i]     = uvIndex[i];
            }
        }else if(sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z) == 3){    // Read vertices and fill in dummy normals, fNormals and tangets.
            vertices.push_back(v);              // Vertex
            normals.push_back(v3d(0.0f));       // Normal
            faceNormals.push_back(v3d(0.0f));   // Face normal
            tangents.push_back(v3d(0.0f));      // Tangent
        } else if(sscanf(line, "vn %f %f %f", &v.x, &v.y, &v.z) == 3){   // Drop the normals.
            // go to next object line
        } else if(sscanf(line, "vt %f %f", &v.x, &v.y) == 2){           // Read UVs.
            v.y = 1.0f - v.y;
            UVs.push_back(v);
        }else if(line[0] == 'v' && line[1] == 'n' ){    last = line[0];
        }else if(sscanf(line, "usemtl %s", str) == 1){  strcpy(usemtl, str);
        }else if(sscanf(line, "o %s", str) == 1){       strcpy(name, str);
        }else if(sscanf(line, "g %s", str) == 1){       strcpy(group, str);
        }else if(sscanf(line, "s %s", str) == 1){
            useSmoothNormals = true;
            if(strstr( str, "off") || strstr(str, "0") ) useSmoothNormals = false;
        }else if(sscanf(line, "mtllib %s", str) == 1){
            unsigned char position = (unsigned char *) line - objSource + strlen(line) + 1;
            loadMaterial(str);
            line = strtok((char *) &objSource[position], "\n");
            continue;
        }
        
        last = line[0];
        line = strtok(NULL, "\n");
    }
    
    if(objSource) delete [] objSource;
    
    // Build the normals and tangent
    {
        for(unsigned int i = 0; i < meshes.size(); ++i){
            ObjMesh *mesh = &meshes[i];
            
            // Accumulate normals and tangent
            for(unsigned int j = 0; j < mesh->tLists.size(); ++j){
                ObjTriangleList *list = &mesh->tLists[j];
                for(unsigned int k = 0; k < list->tIndices.size(); ++k){
                    v3d v1, v2, normal;
                    v1 = vertices[list->tIndices[k].vertexIndex[0]] - vertices[list->tIndices[k].vertexIndex[1]];
                    v2 = vertices[list->tIndices[k].vertexIndex[0]] - vertices[list->tIndices[k].vertexIndex[2]];
                    
                    normal = v3d::normalize(v3d::cross(v1, v2));
                    for(unsigned short e = 0; e < 3; ++e) {
                        memcpy(&faceNormals[list->tIndices[k].vertexIndex[e]], normal.pointer(), sizeof(v3d));
                    }
                    for(unsigned short e = 0; e < 3; ++e){
                        normals[list->tIndices[k].vertexIndex[e]] = normals[list->tIndices[k].vertexIndex[e]] + normal;
                        
                    }
                    
                    if(list->useUVs){
                        v3d tangent;
                        v3d uv1, uv2;
                        float c;
                        
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
        for(unsigned int i = 0; i < meshes.size(); ++i){
            for(unsigned int j = 0; j < meshes[i].vertexData.size(); ++j){
                index = meshes[i].vertexData[j].vIndex;
                
                normals[index] = v3d::normalize(normals[index]);
                if(meshes[i].vertexData[j].uvIndex != -1){
                    tangents[index] = v3d::normalize(tangents[index]);
                }
                
            }
        }
    }
    
    clock_t end = std::clock();
    double elapsedTime = double(end - begin) / CLOCKS_PER_SEC;
    logMessage("Elapsed Time: %f", elapsedTime);
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

void Obj::addMesh(ObjMesh **mesh, ObjTriangleList **tList, char* name, char* usemtl, bool useUVs){
    logMessage("Add new mesh to OBJ");
    meshes.push_back(ObjMesh());
    ObjMesh *tempMesh = *mesh = &meshes.back();
    tempMesh->visible = true;
    
    if(name[0]) tempMesh->name = name;
    else if(usemtl[0]) tempMesh->name = name;
    //if( group[ 0 ] ) strcpy( objmesh->group, group );
    //objmesh->use_smooth_normals = use_smooth_normals;
    tempMesh->tLists.push_back(ObjTriangleList());
    ObjTriangleList *tempList = *tList = &tempMesh->tLists.back();
    
    tempList->mode = GL_TRIANGLES;
    if(useUVs) tempList->useUVs = useUVs;
    //if(usemtl[0]) objTriangleList->objMaterial = obj->getMaterial(usemtl, 1);  ???
    name[0]     = 0;
    usemtl[0]   = 0;
}


void ObjMesh::addVertexData(ObjTriangleList *otl, int vIndex, int uvIndex){
    unsigned short index;
    for(index = 0; index < vertexData.size(); ++index){
        if(vIndex == vertexData[index].vIndex){
            if(uvIndex == -1 || uvIndex == vertexData[index].uvIndex){
                otl->indices.push_back(index);
                return;
            }
        }
    }
    
    ObjVertexData ovd; ovd.vIndex = vIndex; ovd.uvIndex = uvIndex;
    vertexData.push_back(ovd);
    
    otl->indices.push_back(index);
}


#pragma mark Material loading
void Obj::loadMaterial(const char *filename){
    unsigned char* objSource = readOBJFromFile(SRGraphics::getAppContext(), filename);
    
    if(!objSource) return;
    
    ObjMaterial *mat = NULL;
    unsigned short buffSize = 255;
    char *line = strtok((char*)objSource, "\n"),
                 str[255] = {""};
    
    v3d v;
    
    while(line){
        if(!line[0] || line[0] == '#' ){ line = strtok( NULL, "\n" ); continue;
        }else if( sscanf(line, "newmtl %s", str) == 1){
            logMessage("newmtl line %s", str);

            materials.push_back(ObjMaterial());
            mat = &materials.back();
            mat->name = str;
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
        textures.push_back(Texture(SRGraphics::getAppContext(), filename));
    }
}

int Obj::getTextureIndex(const char *filename){
    for(unsigned int i = 0; i < textures.size(); ++i){
        if( !strcmp(textures[i].filename.c_str(), filename) ) return i;
    }
    return -1;
}
