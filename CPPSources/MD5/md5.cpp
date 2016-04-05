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
            md5struct->bindPose = new md5joint();
        }else if(sscanf(line, "numMeshes %d", &md5struct->numMeshes)){
            logMessage("numMeshes %d", md5struct->numMeshes);
            // ????????????????????
            md5struct->meshes.push_back(md5mesh());
        }
        
        line = strtok( NULL, "\n" );
    }
    
    return nullptr;
}