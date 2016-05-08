#include "Obj.h"


Obj::~Obj(){
    logMessage("Obj destructor! %d \n",  meshes.size());
}

sp<Obj> Obj::load(const char *filename){
    
    logGLError();
    
    std::unique_ptr<FileContent> objSource = readBytesFromFile(filename);
#pragma warning throw exception here
    if(!objSource.get()) return nullptr;
    
    sp<ObjMesh>         currentMesh     = nullptr;
    sp<ObjTriangleList> currentTList    = nullptr;
    std::shared_ptr<Obj> data(new Obj());
    v3d v;
    
    char* line = strtok((char*)objSource->content, "\n");
    char last = 0;
    
    unsigned short buffSize = 255;
    char name[buffSize], str[buffSize],usemtl[buffSize], group[buffSize];
    bool useSmoothNormals;
    
    while(line){
        if(!line[0] || line[0] == '#'){
            // go to next object line
        }else if( line[0] == 'f' && line[1] == ' '){    // Read face indices...
            bool useUVs = false; int vertexIndex[3]  = {0, 0, 0}, normalIndex[3] = {0, 0, 0}, uvIndex[3] = {0, 0, 0};
            if(!Obj::readIndices(line, vertexIndex, normalIndex, uvIndex, useUVs)){
                last = line[0];
                line = strtok(NULL, "\n");
                continue;
            }
            if( last != 'f'){
                currentMesh     = std::make_shared<ObjMesh>();
                currentTList    = std::make_shared<ObjTriangleList>();
                data->addMesh(currentMesh, currentTList, name, usemtl, group, useUVs);
            }
            --vertexIndex[0];--vertexIndex[1];--vertexIndex[2]; --uvIndex[0];--uvIndex[1];--uvIndex[2];             // Why? Because vertexIndex in obj file starts from 1! We count from 0...
            for(unsigned short i = 0; i < 3; ++i) currentMesh->addVertexData(currentTList, vertexIndex[i], uvIndex[i]);
            currentTList->tIndices.push_back(ObjTriangleIndex());
            for(int i = 0; i < 3; ++i){
                currentTList->tIndices.back().vertexIndex[i] = vertexIndex[i];
                currentTList->tIndices.back().uvIndex[i]     = uvIndex[i];
            }
        }else if(sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z) == 3){    // Read vertices and fill in dummy normals, fNormals and tangets.
            data->vertices.push_back(v);              // Vertex
            data->normals.push_back(v3d(0.0f));       // Normal
            data->faceNormals.push_back(v3d(0.0f));   // Face normal
            data->tangents.push_back(v3d(0.0f));      // Tangent
        } else if(sscanf(line, "vn %f %f %f", &v.x, &v.y, &v.z) == 3){   // Drop the normals.
            // go to next object line
        } else if(sscanf(line, "vt %f %f", &v.x, &v.y) == 2){           // Read UVs.
            v.y = 1.0f - v.y;
            data->UVs.push_back(v2d(v.x, v.y));
        }else if(line[0] == 'v' && line[1] == 'n' ){    last = line[0];
        }else if(sscanf(line, "usemtl %s", str) == 1){  strcpy(usemtl, str);
        }else if(sscanf(line, "o %s", str) == 1){       strcpy(name, str);
        }else if(sscanf(line, "g %s", str) == 1){       strcpy(group, str);
        }else if(sscanf(line, "s %s", str) == 1){
            useSmoothNormals = true;
            if(strstr( str, "off") || strstr(str, "0") ) useSmoothNormals = false;
        }else if(sscanf(line, "mtllib %s", str) == 1){
            unsigned int position = (unsigned char*)line - objSource->content + strlen(line) + 1;
            Materials::instance()->loadMaterial(str);
            line = strtok((char *) &objSource->content[position], "\n");
            continue;
        }
        
        last = line[0];
        line = strtok(NULL, "\n");
    }
    data->builNormalsAndTangents();
    return data;
}

#pragma mark Helpers
Obj::Obj(){
    
}


bool Obj::readIndices(const char* line, int v[], int n[], int uv[], bool &useUVs){
    if( sscanf(line, "f %d %d %d", &v[0], &v[1], &v[2]) == 3){
        useUVs = false;
        return true;
    }else if( sscanf(line, "f %d//%d %d//%d %d//%d", &v[0], &n[0], &v[1], &n[1], &v[2], &n[2]) == 6){
        useUVs = false;
        return true;
    }else if( sscanf(line, "f %d/%d %d/%d %d/%d", &v[0], &uv[0], &v[1], &uv[1], &v[2], &uv[2]) == 6){
        useUVs = true;
        return true;
    }else{
        if(sscanf( line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v[0], &uv[0], &n[0],
               &v[1], &uv[1], &n[1],
                  &v[2], &uv[2], &n[2]) == 9){
            useUVs = true;
            return true;
        }
    }
    logMessage("Error: Unknown format! %s", line);
    return false;
}

void Obj::addMesh(sp<ObjMesh> mesh, sp<ObjTriangleList> tList, char* name, char* usemtl, char* group, bool useUVs){
//    logMessage("Add new mesh to OBJ %s\n", name);
    meshes.insert(std::pair<string, sp<ObjMesh>>(name, mesh));
    mesh->visible = true;
    if(name[0]) mesh->name = name;
    else if(usemtl[0]) mesh->name = name;
    if( group[ 0 ] ) mesh->group = group;
    //objmesh->use_smooth_normals = use_smooth_normals;
    mesh->tLists.push_back(tList);
    
    tList->mode = GL_TRIANGLES;
    tList->useUVs = useUVs;
    if(usemtl[0]) tList->material = Materials::instance()->getMaterial(usemtl);
    name[0]         = 0;
    usemtl[0]       = 0;
    mesh->weakData  = shared_from_this();
}

void ObjMesh::addVertexData(sp<ObjTriangleList> otl, int vIndex, int uvIndex){
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

void Obj::builNormalsAndTangents(){
    
    for(auto const &meshEntry : meshes){
        sp<ObjMesh> mesh = meshEntry.second;
        for(unsigned int j = 0; j < mesh->tLists.size(); ++j){
            sp<ObjTriangleList> list = mesh->tLists[j];
            for(unsigned int k = 0; k < list->tIndices.size(); ++k){
                v3d v1, v2, normal;
                v1 = vertices[list->tIndices[k].vertexIndex[0]] - vertices[list->tIndices[k].vertexIndex[1]];
                v2 = vertices[list->tIndices[k].vertexIndex[0]] - vertices[list->tIndices[k].vertexIndex[2]];
                
                normal = v3d::normalize(v3d::cross(v1, v2));
                for(unsigned short e = 0; e < 3; ++e) memcpy(&faceNormals[list->tIndices[k].vertexIndex[e]], normal.pointer(), sizeof(v3d));
                
                for(unsigned short e = 0; e < 3; ++e) normals[list->tIndices[k].vertexIndex[e]] = normals[list->tIndices[k].vertexIndex[e]] + normal;
                
                
                if(list->useUVs){
                    v3d tangent;
                    v2d uv1, uv2;
                    float c;
                    uv1 = UVs[list->tIndices[k].uvIndex[2]] - UVs[list->tIndices[k].uvIndex[0]];
                    uv2 = UVs[list->tIndices[k].uvIndex[1]] - UVs[list->tIndices[k].uvIndex[0]];
                    c = 1.0f / (uv1.x * uv2.y - uv2.x * uv1.y);
                    tangent.x = (v1.x * uv2.y + v2.x * uv1.y) * c;
                    tangent.y = (v1.y * uv2.y + v2.y * uv1.y) * c;
                    tangent.z = (v1.z * uv2.y + v2.z * uv1.y) * c;
                    for(unsigned char i = 0; i < 3; ++i)
                        tangents[list->tIndices[k].vertexIndex[i]] = tangents[list->tIndices[k].vertexIndex[i]] + tangent;
                }
            }
        }
    }
    
    unsigned int index;
    for(auto const &meshEntry : meshes){
        sp<ObjMesh> mesh = meshEntry.second;
        for(unsigned int j = 0; j < mesh->vertexData.size(); ++j){
            index = mesh->vertexData[j].vIndex;
            normals[index] = v3d::normalize(normals[index]);
            if(mesh->vertexData[j].uvIndex != -1){
                tangents[index] = v3d::normalize(tangents[index]);
            }
            
        }
    }
}


#pragma mark Mesh building
void Obj::optimizeMesh(string meshName, unsigned int vertexCacheSize){
    sp<ObjMesh> mesh = meshes[meshName];
    if(vertexCacheSize) SetCacheSize(vertexCacheSize);
    
    unsigned short nGroup = 0;
    for(unsigned int i = 0; i < mesh->tLists.size(); ++i){
        PrimitiveGroup *primitiveGroup;
        if( GenerateStrips(&mesh->tLists[i]->indices[0], (unsigned int)mesh->tLists[i]->indices.size(), &primitiveGroup, &nGroup, true)){
            if(primitiveGroup[0].numIndices < mesh->tLists[i]->indices.size()){
//                logMessage("\n%d VS %d\n", mesh->tLists[i]->indices.size(), primitiveGroup[0].numIndices);
                mesh->tLists[i]->mode = GL_TRIANGLE_STRIP;
                unsigned int size = primitiveGroup[0].numIndices * sizeof(unsigned short);
                mesh->tLists[i]->indices.resize(primitiveGroup[0].numIndices);
                memcpy(&mesh->tLists[i]->indices[0], &primitiveGroup[0].indices[0], size);
            }
            delete [] primitiveGroup;
        }
    }
}

sp<ObjMesh> Obj::getMesh(string meshName){
    if(meshes.find(meshName) != meshes.end())
    return meshes[meshName];
    
    return nullptr;
}

std::vector<sp<ObjMesh>> Obj::getAllMeshes(){
    std::vector<sp<ObjMesh>> retVec;
    for (const auto &mesh : meshes) {
        retVec.push_back(mesh.second);
    }
    return retVec;
}

unsigned int Obj::meshesSize()   { return (unsigned int)meshes.size();     }

void Obj::build(){
    logGLError();
    for(auto const &meshEntry : meshes){
        logGLError();
        meshEntry.second->build();
    }
}

void Obj::clear(){
    vertices.clear();
    normals.clear();
    faceNormals.clear();
    tangents.clear();
    UVs.clear();
}
