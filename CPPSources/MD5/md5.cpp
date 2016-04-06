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
        if(sscanf(line, "MD5Version %d", &intVal) == 1){
            logMessage("MD5Version %d\n", intVal);
            if(intVal != 10 ) return nullptr;
        }else if(sscanf(line, "numJoints %d", &md5struct->numJoints) == 1){
            logMessage("numJoints %d \n", md5struct->numJoints);
            // ????????????????????
            sp<md5joint> pose = std::make_shared<md5joint>();
            md5struct->bindPoses.push_back(pose);
        }else if(sscanf(line, "numMeshes %d", &md5struct->numMeshes)){
            logMessage("numMeshes %d", md5struct->numMeshes);
            sp<md5mesh> mesh = std::make_shared<md5mesh>();
            mesh->mode = GL_TRIANGLES;
            mesh->visible = true;
            md5struct->meshes.push_back(mesh);
        }else if(!strncmp(line, "joints {", 8)){
            unsigned int i = 0;
            line = strtok(NULL, "\n");  // same as line = strtok(line, "\n"); ???
            
            while(line[0] != '}'){
                char temp[256];
                if(sscanf(line, "%s %d ( %f %f %f ) ( %f %f %f )",
                         temp,
                          &md5struct->bindPoses[i]->parent,
                          &md5struct->bindPoses[i]->location.x,
                          &md5struct->bindPoses[i]->location.y,
                          &md5struct->bindPoses[i]->location.z,
                          &md5struct->bindPoses[i]->rotation.x,
                          &md5struct->bindPoses[i]->rotation.y,
                          &md5struct->bindPoses[i]->rotation.z
                          ) == 8){
                    
                    md5struct->bindPoses[i]->name = temp;
                    md5struct->bindPoses[i]->rotation.calculateWFromXYZ();
                    ++i;
                }
                line = strtok(NULL, "\n");
            }
        }else if(!strncmp(line, "mesh {", 6)){
            md5vertex vertex;
            md5triangle triangle;
            md5weight   weight;
            
            line = strtok(NULL, "\n");
            
            while( line[0] != '}' ){
                char temp[256];
                if(sscanf(line, " shader \"%[^\"]", temp) == 1){
                    md5struct->meshes[meshIndex]->shader = temp;
                    // GOTO NEXT MESH?????
                }else if(sscanf(line, " numverts %d", &md5struct->meshes[meshIndex]->nVertex) == 1){
                    md5struct->meshes[meshIndex]->vertices.resize(md5struct->meshes[meshIndex]->nVertex);
                }else if(sscanf(line, " vert %d ( %f %f ) %d %d",
                                &intVal,
                                &vertex.uv.x, &vertex.uv.y, &vertex.start, &vertex.count) == 5){
                    md5struct->meshes[meshIndex]->vertices[intVal] = vertex;
                }
            }
            
        }
        
        line = strtok( NULL, "\n" );
    }
    
    return nullptr;
}