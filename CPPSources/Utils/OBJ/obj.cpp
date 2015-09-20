#include "obj.h"
#include "Game.h"

Obj::Obj(const char* fileName){

    std::unique_ptr<FileContent> objSource = readOBJFromFile(Game::getAppContext(), fileName);
#pragma warning throw exception here
    if(!objSource.get()) return;
    
    std::shared_ptr<ObjMesh> currentMesh = nullptr;
    std::shared_ptr<ObjTriangleList> currentTList = nullptr;
    
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
            bool useUVs; int vertexIndex[3]  = {0, 0, 0}, normalIndex[3] = {0, 0, 0}, uvIndex[3] = {0, 0, 0};
            if(!Obj::readIndices(line, vertexIndex, normalIndex, uvIndex, useUVs)){
                last = line[0];
                line = strtok(NULL, "\n");
                continue;
            }
            if( last != 'f') addMesh(currentMesh, currentTList, name, usemtl, useUVs);
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
            unsigned char position = ( char *) line - objSource->content + strlen(line) + 1;
            loadMaterial(str);
            line = strtok((char *) &objSource->content[position], "\n");
            continue;
        }
        
        last = line[0];
        line = strtok(NULL, "\n");
    }
    builNormalsAndTangents();
}



#pragma mark Helpers
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

void Obj::addMesh(std::shared_ptr<ObjMesh> &mesh, std::shared_ptr<ObjTriangleList> &tList, char* name, char* usemtl, bool useUVs){
    logMessage("Add new mesh to OBJ");
    meshes.push_back(std::shared_ptr<ObjMesh>(new ObjMesh()));
    mesh = meshes.back();
    mesh->visible = true;
    if(name[0]) mesh->name = name;
    else if(usemtl[0]) mesh->name = name;
    //if( group[ 0 ] ) strcpy( objmesh->group, group );
    //objmesh->use_smooth_normals = use_smooth_normals;
    mesh->tLists.push_back(std::shared_ptr<ObjTriangleList>(new ObjTriangleList()));
    tList = mesh->tLists.back();
    
    tList->mode = GL_TRIANGLES;
    if(useUVs) tList->useUVs = useUVs;
    if(usemtl[0]) tList->material = getMaterial(usemtl);
    name[0]     = 0;
    usemtl[0]   = 0;
}

void ObjMesh::addVertexData(std::shared_ptr<ObjTriangleList> otl, int vIndex, int uvIndex){
    unsigned short index;
    if(vIndex < 0){
        int qweqwe = 0;
    }
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
    for(unsigned int i = 0; i < meshes.size(); ++i){
        std::shared_ptr<ObjMesh> mesh = meshes[i];
        for(unsigned int j = 0; j < mesh->tLists.size(); ++j){
            std::shared_ptr<ObjTriangleList> list = mesh->tLists[j];
            for(unsigned int k = 0; k < list->tIndices.size(); ++k){
                v3d v1, v2, normal;
                v1 = vertices[list->tIndices[k].vertexIndex[0]] - vertices[list->tIndices[k].vertexIndex[1]];
                v2 = vertices[list->tIndices[k].vertexIndex[0]] - vertices[list->tIndices[k].vertexIndex[2]];
                    
                normal = v3d::normalize(v3d::cross(v1, v2));
                for(unsigned short e = 0; e < 3; ++e) memcpy(&faceNormals[list->tIndices[k].vertexIndex[e]], normal.pointer(), sizeof(v3d));
                
                for(unsigned short e = 0; e < 3; ++e) normals[list->tIndices[k].vertexIndex[e]] = normals[list->tIndices[k].vertexIndex[e]] + normal;
                
                    
                if(list->useUVs){
                    v3d tangent;
                    v3d uv1, uv2;
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
    for(unsigned int i = 0; i < meshes.size(); ++i){
        for(unsigned int j = 0; j < meshes[i]->vertexData.size(); ++j){
            index = meshes[i]->vertexData[j].vIndex;
            normals[index] = v3d::normalize(normals[index]);
            if(meshes[i]->vertexData[j].uvIndex != -1){
                tangents[index] = v3d::normalize(tangents[index]);
            }
                
        }
    }
}


#pragma mark Mesh building
void Obj::buildMesh(unsigned int meshIndex){
    updateBoundMesh(meshIndex);
    buildVBOMesh(meshIndex);
    std::shared_ptr<ObjMesh> mesh = meshes[meshIndex];

    glGenVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);
    
    mesh->setAttributes();
    
    if(mesh->tLists.size() == 1){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->tLists[0]->vbo);
    }
    
    glBindVertexArray(0);
}

void Obj::optimizeMesh(unsigned int meshIndex, unsigned int vertexCacheSize){
    std::shared_ptr<ObjMesh> mesh = meshes[meshIndex];
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

unsigned int Obj::drawMesh(unsigned int meshIndex){
    return meshes[meshIndex]->draw();
}

unsigned int Obj::materialsSize(){ return (unsigned int)materials.size();  }
unsigned int Obj::meshesSize()   { return (unsigned int)meshes.size();     }
unsigned int Obj::texturesSize() { return (unsigned int)textures.size();   }
void         Obj::generateTextureID(unsigned int textureIndex, unsigned int flags, unsigned int filter){
    textures[textureIndex]->generateID(flags, filter);
}
void        Obj::SetMaterialProgram(unsigned int matIndex, BindAttribCallback bindCallback){
    materials[matIndex]->program = ShaderLibrary::instance()->getProgram("defaultPerPixel");
    materials[matIndex]->program->bindAttribCallback = bindCallback;
}

void        Obj::SetMaterialCallback(unsigned int matIndex, DrawCallback callback){
    materials[matIndex]->materialDrawCalback = callback;
}

RenderObjectType Obj::renderObjectType(unsigned int meshIndex){
    std::shared_ptr<ObjMesh> mesh = meshes[meshIndex];
    if(mesh->currentMaterial){
        float dissolve = mesh->currentMaterial->dissolve;
        if(dissolve == 0.0f){
            return ALPHA_TESTED;
        }else if(dissolve == 1.0f){
            return SOLID;
        }else return TRANSPARENT;
    }
    return SOLID;
}




void Obj::updateBoundMesh(unsigned int meshIndex){
    std::shared_ptr<ObjMesh> mesh = meshes[meshIndex];
    
    mesh->min.x = mesh->min.y = mesh->min.z = 9999.99f;
    mesh->max.x = mesh->max.y = mesh->max.z = -9999.99f;
    
    unsigned int index;
    for(unsigned int i = 0; i < mesh->vertexData.size(); ++i){
        index = mesh->vertexData[i].vIndex;
        updateMin(mesh->min, vertices[index]);
        updateMax(mesh->max, vertices[index]);
    }

    mesh->location = (mesh->min + mesh->max) * 0.5f;
    mesh->dimension = mesh->max - mesh->min;
//    mesh->radius =  mesh->dimension.x >= mesh->dimension.y ?
//                    mesh->dimension.x :
//                    mesh->dimension.y;
//    mesh->radius =  mesh->radius >= mesh->dimension.x ?
//                    mesh->radius * 0.5f :
//                    mesh->dimension.z * 0.5f;

    mesh->radius = v3d::length(mesh->max - mesh->min) * 0.5f;
}

void Obj::updateMin(v3d &min, v3d &vertex){
    if(vertex.x < min.x) min.x = vertex.x;
    if(vertex.y < min.y) min.y = vertex.y;
    if(vertex.z < min.z) min.z = vertex.z;
}

void Obj::updateMax(v3d &max, v3d &vertex){
    if(vertex.x > max.x) max.x = vertex.x;
    if(vertex.y > max.y) max.y = vertex.y;
    if(vertex.z > max.z) max.z = vertex.z;
}

void Obj::buildVBOMesh(unsigned int meshIndex){
    std::shared_ptr<ObjMesh> mesh = meshes[meshIndex];
    
    unsigned int v3dSize = sizeof(v3d);
    mesh->stride = v3dSize;
    mesh->stride += v3dSize;
    mesh->stride += v3dSize;
    
    if(mesh->vertexData[0].uvIndex != -1){
        mesh->stride += v3dSize;
        mesh->stride += v3dSize;
    }
    
    mesh->size = (unsigned int)mesh->vertexData.size() * mesh->stride;
    
    unsigned char *vertexArray = (unsigned char*) malloc(mesh->size);
    unsigned char *vertexStart = vertexArray;
    
    unsigned int index;
    for(unsigned int i = 0; i < mesh->vertexData.size(); ++i){
        index = mesh->vertexData[i].vIndex;
        memcpy(vertexArray, &vertices[index], v3dSize);
        // Center the pivot
//        v3d centerThePivot = vertices[index] - mesh->location;      // ??????????
//        memcpy(vertexArray, &centerThePivot, v3dSize);              // ??????????
        vertexArray += v3dSize;
        memcpy(vertexArray, &normals[index], v3dSize);
        vertexArray += v3dSize;
        memcpy(vertexArray, &faceNormals[index], v3dSize);
        vertexArray += v3dSize;
        if(mesh->vertexData[0].uvIndex != -1){
            memcpy(vertexArray, &UVs[mesh->vertexData[i].uvIndex], v3dSize);
            vertexArray += v3dSize;
            memcpy(vertexArray, &tangents[index], v3dSize);
            vertexArray += v3dSize;
        }
    }
    
    glGenBuffers(1, &mesh->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh->size, vertexStart, GL_STATIC_DRAW);
    free(vertexStart);
    
//    logMessage("Mesh vertices vbo:  ARRAY  %d\n", mesh->vbo);
    
    unsigned int offset = 0;
    mesh->offset[0] = offset;
    offset += v3dSize;
    mesh->offset[1] = offset;
    offset += v3dSize;
    mesh->offset[2] = offset;
    if(mesh->vertexData[0].uvIndex != -1){
        offset += v3dSize;
        mesh->offset[3] = offset;
        offset += v3dSize;
        mesh->offset[4] = offset;
    }
    
    for(unsigned int i = 0; i < mesh->tLists.size(); ++i){
        glGenBuffers(1, &mesh->tLists[i]->vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->tLists[i]->vbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->tLists[i]->indices.size() * sizeof(unsigned short), &mesh->tLists[i]->indices[0], GL_STATIC_DRAW);
//        logMessage("tList VBO: %d\n", mesh->tLists[i].vbo);
    }
}

#pragma mark Material loading
void Obj::loadMaterial(const char *filename){
    std::unique_ptr<FileContent> objSource = readOBJFromFile(Game::getAppContext(), filename);
    
    if(!objSource.get()) return;
    
    std::shared_ptr<ObjMaterial> mat = NULL;
    char *line = strtok((char*)objSource->content, "\n"),
                 str[255] = {""};
    
    v3d v;
    
    while(line){
        if(!line[0] || line[0] == '#' ){ line = strtok( NULL, "\n" ); continue;
        }else if( sscanf(line, "newmtl %s", str) == 1){
//            logMessage("newmtl line %s", str);
            materials.push_back(std::shared_ptr<ObjMaterial>(new ObjMaterial()));
            mat = materials.back();
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
            mat->mapAmbient = str;
            addTexture(str);
        }else if(sscanf(line, "map_Kd %s", str) == 1){
            mat->mapDiffuse = str;
            addTexture(str);
        }else if(sscanf(line, "map_Ks %s", str) == 1){
            mat->mapSpecular = str;
            addTexture(str);
        }else if(sscanf(line, "map_Tr %s", str) == 1){
            mat->mapTranslucency = str;
            addTexture(str);
        }else if( sscanf( line, "map_disp %s", str ) == 1 ||
                 sscanf( line, "map_Disp %s", str ) == 1 ||
                 sscanf( line, "disp %s"    , str ) == 1 ){
            mat->mapDisp = str;
            addTexture(str);
        }else if( sscanf( line, "map_bump %s", str ) == 1 ||
                 sscanf( line, "map_Bump %s", str ) == 1 ||
                 sscanf( line, "bump %s"	, str ) == 1 ){
            mat->mapBump = str;
            addTexture(str);
        }
        
        
        line = strtok( NULL, "\n" );
    }
    
    logMessage("%s", objSource->content);
}

void Obj::addTexture(const char *filename){
    if(getTextureIndex(filename) < 0){
        textures.push_back(Texture::load(Game::getAppContext(), filename, TextureSource::PNG));
    }
}

int Obj::getTextureIndex(const char *filename){
    for(unsigned int i = 0; i < textures.size(); ++i){
        if( !strcmp(textures[i]->filename.c_str(), filename) ) return i;
    }
    return -1;
}

std::shared_ptr<ObjMaterial> Obj::getMaterial(const char *name){
    std::shared_ptr<ObjMaterial> mat = nullptr;
    for(unsigned int i = 0; i < materials.size(); ++i){
        if(!strcmp(materials[i]->name.c_str(), name)){
            mat = materials[i];
            break;
        }
    }
    return mat;
}

void Obj::buildMaterial(unsigned int matIndex, std::shared_ptr<ShaderProgram> program){
    std::shared_ptr<ObjMaterial> mat = materials[matIndex];

    if(textures.size() > 0){
        int index;
        if(mat->mapAmbient[0]){
            index = getTextureIndex(mat->mapAmbient.c_str());
            if(index >= 0) mat->tAmbient = textures[index];
        }
        
        if(mat->mapDiffuse[0]){
            index = getTextureIndex(mat->mapDiffuse.c_str());
            if(index >= 0) mat->tDiffuse = textures[index];
        }
        
        if(mat->mapSpecular[0]){
            index = getTextureIndex(mat->mapSpecular.c_str());
            if(index >= 0) mat->tSpecular = textures[index];
        }
        
        if(mat->mapTranslucency[0]){
            index = getTextureIndex(mat->mapTranslucency.c_str());
            if(index >= 0) mat->tTranslucency = textures[index];
        }
        
        if(mat->mapDisp[0]){
            index = getTextureIndex(mat->mapDisp.c_str());
            if(index >= 0) mat->tDisp = textures[index];
        }
        
        if(mat->mapBump[0]){
            index = getTextureIndex(mat->mapBump.c_str());
            if(index >= 0) mat->tBump = textures[index];
        }        
    }
    if(program) mat->program = program;
}



