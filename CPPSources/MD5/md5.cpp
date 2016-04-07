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
    
    unsigned int meshIndex = 0;
    
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
            sp<md5mesh> mesh = std::make_shared<md5mesh>();
            mesh->mode = GL_TRIANGLES;
            mesh->visible = true;
            md5struct->meshes.push_back(mesh);
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
            md5vertex vertex;
            md5triangle triangle;
            md5weight   weight;
            
            line = strtok(NULL, "\n");
            
            while( line[0] != '}' ){
                logMessage("%s\n", line);
                char temp[256];
                if(sscanf(line, " shader \"%[^\"]", temp) == 1){
                    md5struct->meshes[meshIndex]->shader = temp;
                    line = strtok( NULL, "\n" );
                    continue;
                }else if(sscanf(line, " numverts %d", &md5struct->meshes[meshIndex]->nVertex) == 1){
                    md5struct->meshes[meshIndex]->vertices.reserve(md5struct->meshes[meshIndex]->nVertex);
                }else if(sscanf(line, " vert %d ( %f %f ) %d %d",
                                &intVal,
                                &vertex.uv.x, &vertex.uv.y, &vertex.start, &vertex.count) == 5){
                    md5struct->meshes[meshIndex]->vertices.push_back(vertex);
                }else if(sscanf(line, " numtris %d", &md5struct->meshes[meshIndex]->nTriangle) == 1){
                    md5struct->meshes[meshIndex]->nIndice   = md5struct->meshes[meshIndex]->nTriangle * 3;
                    md5struct->meshes[meshIndex]->triangles.reserve(md5struct->meshes[meshIndex]->nTriangle );
                }else if(sscanf(line, "tri %d %hu %hu %hu", &intVal, &triangle.indices[2], &triangle.indices[1], &triangle.indices[0] ) == 4){
                    md5struct->meshes[meshIndex]->triangles.push_back(triangle);
                }else if(sscanf(line, "numweights %d", &md5struct->meshes[meshIndex]->nWeight) == 1){
                    md5struct->meshes[meshIndex]->weights.reserve(md5struct->meshes[meshIndex]->nWeight);
                }else if(sscanf(line, " weight %d %d %f ( %f %f %f )", &intVal, &weight.joint, &weight.bias, &weight.location.x, &weight.location.y, &weight.location.z) == 6){
                    md5struct->meshes[meshIndex]->weights.push_back(weight);
                }
            
                line = strtok(NULL, "\n");
            }
//            
//            unsigned int s = md5struct->meshes[meshIndex]->nIndice * sizeof(unsigned short);
//            md5struct->meshes[meshIndex]->indices[0] = md5struct->meshes[meshIndex]->triangles[0];

            
        }
        
        line = strtok( NULL, "\n" );
    }
    
    return nullptr;
}