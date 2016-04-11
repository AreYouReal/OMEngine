#include "md5.hpp"
#include <memory.h>
#include "NvTriStrip.h"

using namespace md5;

sp<MD5> MD5::loadMesh(string filename){
    
    up<FileContent> objSource = readBytesFromFile(filename.c_str());
    if(!objSource.get()) return nullptr;
    
    auto md5 = std::make_shared<MD5>();
    
    md5->distance = md5->scale.x = md5->scale.y = md5->scale.z = 1.0f;
    md5->visible = true;
    
    char* line = strtok((char*)objSource->content, "\n");
    
    int intVal = 0;
    
    while(line){
        logMessage("%s\n", line);
        
        if(sscanf(line, "MD5Version %d", &intVal) == 1){
            logMessage("MD5Version %d\n", intVal);
            if(intVal != 10 ) return nullptr;
        }else if(sscanf(line, "numJoints %d", &md5->numJoints) == 1){
            logMessage("numJoints %d \n", md5->numJoints);
            md5->bindPose.reserve(md5->numJoints);
        }else if(sscanf(line, "numMeshes %d", &md5->numMeshes)){
            logMessage("numMeshes %d\n", md5->numMeshes);
            md5->meshes.reserve(md5->numMeshes);
        }else if(!strncmp(line, "joints {", 8)){
            line = strtok(NULL, "\n");  // same as line = strtok(line, "\n"); ???
            
            while(line[0] != '}'){
                logMessage("%s\n", line);
                char temp[256];
                
                Joint   joint;
                
                if(sscanf(line, "%s %d ( %f %f %f ) ( %f %f %f )",
                         temp,
                          &joint.parent,
                          &joint.location.x,
                          &joint.location.y,
                          &joint.location.z,
                          &joint.rotation.x,
                          &joint.rotation.y,
                          &joint.rotation.z
                          ) == 8){
                    joint.rotation.calculateWFromXYZ();
                    joint.name = temp;
                    md5->bindPose.push_back(joint);
                }
                line = strtok(NULL, "\n");
            }
        }else if(!strncmp(line, "mesh {", 6)){
            line = strtok(NULL, "\n");
            md5->meshes.push_back(loadMeshData(line));
        }
        
        line = strtok( NULL, "\n" );
    }
    
    return md5;
}


sp<Mesh> MD5::loadMeshData(char *line){
    Vertex vertex;
    Triangle triangle;
    Weight   weight;
    
    sp<Mesh> mesh = std::make_shared<Mesh>();
    mesh->mode = GL_TRIANGLES;
    mesh->visible = true;
    
    int intVal = 0;
    
    while( line[0] != '}' ){

        
        logMessage("%s\n", line);
        char temp[256];
        if(sscanf(line, " shader \"%[^\"]", temp) == 1){
            mesh->shader = temp;
            line = strtok( NULL, "\n" );
            continue;
        }else if(sscanf(line, " numverts %d", &mesh->nVertex) == 1){
            mesh->vertices.reserve(mesh->nVertex);
        }else if(sscanf(line, " vert %d ( %f %f ) %d %d",
                        &intVal,
                        &vertex.uv.x, &vertex.uv.y, &vertex.start, &vertex.count) == 5){
            mesh->vertices.push_back(vertex);
        }else if(sscanf(line, " numtris %d", &mesh->nTriangle) == 1){
            mesh->nIndices   = mesh->nTriangle * 3;
            mesh->triangles.reserve(mesh->nTriangle );
        }else if(sscanf(line, " tri %d %hu %hu %hu", &intVal, &triangle.indices[2], &triangle.indices[1], &triangle.indices[0] ) == 4){
            mesh->triangles.push_back(triangle);
        }else if(sscanf(line, "numweights %d", &mesh->nWeight) == 1){
            mesh->weights.reserve(mesh->nWeight);
        }else if(sscanf(line, " weight %d %d %f ( %f %f %f )", &intVal, &weight.joint, &weight.bias, &weight.location.x, &weight.location.y, &weight.location.z) == 6){
            mesh->weights.push_back(weight);
        }
        
        line = strtok(NULL, "\n");
    }

    mesh->indices.resize(mesh->nIndices);
    memcpy(&mesh->indices[0], &mesh->triangles[0].indices[0], mesh->nIndices * sizeof(unsigned short));
    
    return mesh;
}



void MD5::optimize(unsigned int vertexCacheSize){

    unsigned short nGroup = 0;

    if(vertexCacheSize) SetCacheSize(vertexCacheSize);
    
    for(auto &mesh : meshes){
        PrimitiveGroup *pg;
        if(GenerateStrips(&mesh->indices[0], mesh->nIndices, &pg, &nGroup, true)){
            if(pg[0].numIndices < mesh->nIndices){
                mesh->mode = GL_TRIANGLE_STRIP;
                mesh->nIndices = pg[0].numIndices;
                unsigned int size = pg[0].numIndices * sizeof(unsigned short);
                mesh->indices.resize(mesh->nIndices);
                memcpy(&mesh->indices[0], &pg[0].indices[0], size);
            }
            delete [] pg;
        }
    }
}

void MD5::build(){
    for(auto &mesh : meshes){
        mesh->buildVAO();
    }
    
    setPose();
}

void MD5::setPose(){
    unsigned int i = 0, j, k;
    
    for(auto &mesh : meshes){
        v3d *vertexArray = (v3d*)&mesh->vertexData[0];
        v3d *normalArray = (v3d*)&mesh->vertexData[mesh->offset[1]];
        v3d *tangentArray = (v3d*)&mesh->vertexData[mesh->offset[3]];
        
        v2d *uvArray = (v2d*)&mesh->vertexData[mesh->offset[2]];
        
        memset(vertexArray, 0, mesh->offset[1]);
        memset(normalArray, 0, mesh->offset[1]);
        memset(tangentArray, 0, mesh->offset[1]);
        
        for(uint j = 0; j < mesh->nVertex; ++j){
            Vertex vertex = mesh->vertices[j];
            for(ushort i = 0; i < vertex.count; ++i){
                v3d location    (0, 0, 0);
                v3d normal      (0, 0, 0);
                v3d tangent     (0, 0, 0);
                
                Weight md5weight = mesh->weights[vertex.start + i];
                Joint md5joint = bindPose[md5weight.joint];
                
                logMessage("[%f %f %f] [%f %f %f %f]\n", md5joint.location.x, md5joint.location.y, md5joint.location.z, md5joint.rotation.x, md5joint.rotation.y, md5joint.rotation.z, md5joint.rotation.w );
                
                m4d rotMat = md5joint.rotation.matrix();
                
                location = md5weight.location * rotMat;
                normal = md5weight.normal * rotMat;
                tangent = md5weight.tangent * rotMat;
                
                vertexArray[j].x += (md5joint.location.x + location.x) * md5weight.bias;
                vertexArray[j].y += (md5joint.location.y + location.y) * md5weight.bias;
                vertexArray[j].z += (md5joint.location.z + location.z) * md5weight.bias;
                
                normalArray[j].x += normal.x * md5weight.bias;
                normalArray[j].y += normal.y * md5weight.bias;
                normalArray[j].z += normal.z * md5weight.bias;
                
                tangentArray[j].x += tangent.x * md5weight.bias;
                tangentArray[j].y += tangent.y * md5weight.bias;
                tangentArray[j].z += tangent.z * md5weight.bias;
            }
            uvArray[j].x = vertex.uv.x;
            uvArray[j].y = vertex.uv.y;
        }
        
        
    }
}

void Mesh::buildVAO(){
    buildVBO();
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    setAttributes();
    glBindVertexArray(0);
}

void Mesh::buildVBO(){
    unsigned short sizeOfV3D = sizeof(v3d); // Position, Normal, Tangent0
    unsigned short sizeOfV2D = sizeof(v2d); // TexCoord0
    size = vertices.size() * (3 * sizeOfV3D + sizeOfV2D) ;
    vertexData.reserve(size);
    offset[0] = 0;
    offset[1] = nVertex * sizeOfV3D;
    offset[2] = offset[1] + nVertex * sizeOfV3D;
    offset[3] = offset[2] + nVertex * sizeOfV2D;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, &vertexData[0], GL_DYNAMIC_DRAW);
    glGenBuffers(1, &vboIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices * sizeof(unsigned char), &indices[0], GL_STATIC_DRAW);
}

void Mesh::setAttributes(){
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (char*)NULL + offset[0]);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (char*)NULL + offset[1]);
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (char*)NULL + offset[2]);
    
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (char*)NULL + offset[3]);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
}