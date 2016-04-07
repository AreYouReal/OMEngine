#include "md5.hpp"
#include <memory.h>


sp<md5> md5::loadMesh(string filename){
    
    up<FileContent> objSource = readBytesFromFile(filename.c_str());
    if(!objSource.get()) return nullptr;
    
    auto md5struct = std::make_shared<md5>();
    
    md5struct->distance = md5struct->scale.x = md5struct->scale.y = md5struct->scale.z = 1.0f;
    md5struct->visible = true;
    
    char* line = strtok((char*)objSource->content, "\n");
    
    int intVal = 0;
    
    while(line){
        logMessage("%s\n", line);
        
        if(sscanf(line, "MD5Version %d", &intVal) == 1){
            logMessage("MD5Version %d\n", intVal);
            if(intVal != 10 ) return nullptr;
        }else if(sscanf(line, "numJoints %d", &md5struct->numJoints) == 1){
            logMessage("numJoints %d \n", md5struct->numJoints);
            md5struct->bindPose.reserve(md5struct->numJoints);
        }else if(sscanf(line, "numMeshes %d", &md5struct->numMeshes)){
            logMessage("numMeshes %d\n", md5struct->numMeshes);
            md5struct->meshes.reserve(md5struct->numMeshes);
        }else if(!strncmp(line, "joints {", 8)){
            line = strtok(NULL, "\n");  // same as line = strtok(line, "\n"); ???
            
            while(line[0] != '}'){
                logMessage("%s\n", line);
                char temp[256];
                
                sp<md5joint>    joint = std::make_shared<md5joint>();
                
                if(sscanf(line, "%s %d ( %f %f %f ) ( %f %f %f )",
                         temp,
                          &joint->parent,
                          &joint->location.x,
                          &joint->location.y,
                          &joint->location.z,
                          &joint->rotation.x,
                          &joint->rotation.y,
                          &joint->rotation.z
                          ) == 8){
                    joint->rotation.calculateWFromXYZ();
                    joint->name = temp;
                    md5struct->bindPose.push_back(joint);
                }
                line = strtok(NULL, "\n");
            }
        }else if(!strncmp(line, "mesh {", 6)){
            line = strtok(NULL, "\n");
            md5struct->meshes.push_back(loadMeshData(line));
        }
        
        line = strtok( NULL, "\n" );
    }
    
    return nullptr;
}


sp<md5mesh> md5::loadMeshData(char *line){
    md5vertex vertex;
    md5triangle triangle;
    md5weight   weight;
    
    sp<md5mesh> mesh = std::make_shared<md5mesh>();
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
            mesh->nIndice   = mesh->nTriangle * 3;
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
    
    for(auto const &triangle : mesh->triangles){
        for(unsigned short i = 0; i < 3; ++i){
            mesh->indices.push_back(triangle.indices[i]);
        }
    }
    return mesh;
}