#include "md5.hpp"
#include <memory.h>
#include "NvTriStrip.h"

#include "Illuminator.hpp"

using namespace md5;


#pragma MD5 related

sp<MD5> MD5::loadMesh(string filename){
    
    up<FileContent> objSource = readBytesFromFile(filename.c_str());
    if(!objSource.get()) return nullptr;
    
    auto md5 = std::make_shared<MD5>();
    
    md5->distance = md5->scale.x = md5->scale.y = md5->scale.z = 1.0f;
    md5->visible = true;
    
    char* line = strtok((char*)objSource->content, "\n");
    
    int intVal = 0;
    
    while(line){
//        logMessage("%s\n", line);
        
        if(sscanf(line, "MD5Version %d", &intVal) == 1){
//            logMessage("MD5Version %d\n", intVal);
            if(intVal != 10 ) return nullptr;
        }else if(sscanf(line, "numJoints %d", &md5->numJoints) == 1){
//            logMessage("numJoints %d \n", md5->numJoints);
            md5->mBindPose.reserve(md5->numJoints);
        }else if(sscanf(line, "numMeshes %d", &md5->numMeshes)){
//            logMessage("numMeshes %d\n", md5->numMeshes);
            md5->meshes.reserve(md5->numMeshes);
        }else if(!strncmp(line, "joints {", 8)){
            line = strtok(NULL, "\n");  // same as line = strtok(line, "\n"); ???
            
            while(line[0] != '}'){
//                logMessage("%s\n", line);
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
                    md5->mBindPose.push_back(joint);
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
    
    int intVal = 0;
    
    while( line[0] != '}' ){
//        logMessage("%s\n", line);
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

sp<Action> MD5::loadAction(string name, string filename){

    logMessage("LOAD ACTION: %s, %s\n", name.c_str(), filename.c_str());
    
    up<FileContent> content = readBytesFromFile(filename.c_str());
    if(!content.get()) return nullptr;
    
    logMessage("file loaded!\n");
    
    sp<Action> action = std::make_shared<Action>();
    action->name = name;
    
    char* line = strtok((char *)content->content, "\n");
    int intVal = 0;
    
    while(line){

        if(sscanf(line, "MD5Version %d", &intVal) == 1){
            if(intVal != 10){
                logMessage("ERROR! MD5Version %d is not supported!\n", intVal );
                return nullptr;
            }
        }else if(sscanf(line, "numFrames %d", &action->nFrames) == 1){
            action->frame.resize(action->nFrames);
            for(auto &vecEntry : action->frame){
                vecEntry.resize(numJoints);
            }
        }else if(sscanf(line, "numJoints %d", &intVal) == 1){
            if(numJoints != intVal){
                logMessage("ERROR! num of MD5 joints and action joints is different!\n");
                return nullptr;
            }
            action->pose.reserve(numJoints);
        }else if(sscanf(line, "frameRate %d", &intVal) == 1){
            action->fps = 1.0f / intVal;
        }else if(sscanf(line, "frame %d", &intVal)){
            Joint *joint = &action->frame[intVal][0];
            line = strtok(NULL, "\n");
            for(unsigned int i = 0; i < numJoints; ++i){
                if(sscanf(line, " %f %f %f %f %f %f", &joint[i].location.x,
                          &joint[i].location.y,
                          &joint[i].location.z,
                          &joint[i].rotation.x,
                          &joint[i].rotation.y,
                          &joint[i].rotation.z) == 6){
                    joint[i].name = mBindPose[i].name;
                    joint[i].rotation.calculateWFromXYZ();
                }
                line = strtok(NULL, "\n");
            }
            
            v3d location;
            q4d rotation;
            
            for(unsigned int i = 0; i < numJoints; ++i){
                if(mBindPose[i].parent > -1){
                    Joint *joint = &action->frame[intVal][mBindPose[i].parent];
                    m4d rotMat = joint->rotation.matrix();
                    location = action->frame[intVal][i].location * rotMat;
                    action->frame[intVal][i].location.x = location.x + joint->location.x;
                    action->frame[intVal][i].location.y = location.y + joint->location.y;
                    action->frame[intVal][i].location.z = location.z + joint->location.z;
                    rotation = joint->rotation * action->frame[intVal][i].rotation;
                    rotation.normalize();
                    action->frame[intVal][i].rotation = rotation;
                }
            }
        }
        
        line = strtok(NULL, "\n");
        
    }
//    logMessage("LOAD ACTION -> INSERT: %s, %s\n", name.c_str(), filename.c_str());

    actions.insert(std::pair<string, sp<Action>>(action->name, action));
    return actions[action->name];
}

sp<Action> MD5::getAction(const string name){
    if(actions.find(name) == actions.end()){
        return nullptr;
    }
    return actions[name];
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
        mesh->initMaterial();
    }
    
    setPose();
    buildPoseWeightedNormalsTangents();
    setPose();
    updateBoundMesh();
}

void MD5::draw(){
    setPose();
    if(visible && distance){
        for(auto &mesh : meshes){
            mesh->draw();
        }
    }
}

void MD5::setPose(){
    switch (mAnimType) {
        case SINGLE_ACTION:
            if(currentActions.size() != 0)
                mBindPose = currentActions.back()->pose;
            break;
        case BLEND_ACTIONS:
            if(currentActions.size() >= 2){
                blendActions(currentActions[0]->pose, currentActions[1]->pose, Action::InterpolationMethod::FRAME, 0.5f);
            }
            break;
        case ADD_ACTIONS:
            if(currentActions.size() >= 2){
                addActions(currentActions[0], currentActions[1], Action::InterpolationMethod::FRAME, 0.5f);
            }
            break;
        default:
            break;
    }
    
    
    for(auto &mesh : meshes){
        v3d *vertexArray = (v3d*)&mesh->vertexData[0];
        v3d *normalArray = (v3d*)&mesh->vertexData[mesh->offset[1]];
        v3d *tangentArray = (v3d*)&mesh->vertexData[mesh->offset[3]];
        
        v2d *uvArray = (v2d*)&mesh->vertexData[mesh->offset[2]];
        
        memset(vertexArray, 0, mesh->offset[1]);
        memset(normalArray, 0, mesh->offset[1]);
        memset(tangentArray, 0, mesh->offset[1]);
        
        for(unsigned int j = 0; j < mesh->nVertex; ++j){
            Vertex vertex = mesh->vertices[j];
            for(unsigned short i = 0; i < vertex.count; ++i){
                v3d location    (0, 0, 0);
                v3d normal      (0, 0, 0);
                v3d tangent     (0, 0, 0);
                
                Weight md5weight = mesh->weights[vertex.start + i];
                Joint md5joint = mBindPose[md5weight.joint];
                
//                logMessage("[%f %f %f] [%f %f %f %f]\n", md5joint.location.x, md5joint.location.y, md5joint.location.z, md5joint.rotation.x, md5joint.rotation.y, md5joint.rotation.z, md5joint.rotation.w );
                
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
        
        glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->size, &mesh->vertexData[0]);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MD5::buildPoseWeightedNormalsTangents(){
    for(auto &mesh : meshes){
        v3d *vertexArray = (v3d*)&mesh->vertexData[0];
        for(unsigned int i = 0; i < mesh->nVertex; ++i){
            memset(&mesh->vertices[i].normal, 0, sizeof(v3d));
            memset(&mesh->vertices[i].tangent, 0, sizeof(v3d));
        }
        
        for(auto &triangle : mesh->triangles){
            v3d v1, v2, normal;
            
            v1 = vertexArray[triangle.indices[0]] - vertexArray[triangle.indices[1]];
            v2 = vertexArray[triangle.indices[0]] - vertexArray[triangle.indices[2]];
            normal = v3d::cross(v1, v2).normalize();
            
            // Flat normals
//            memcpy(&mesh->vertices[triangle.indices[0]].normal, &normal, sizeof(v3d));
//            memcpy(&mesh->vertices[triangle.indices[1]].normal, &normal, sizeof(v3d));
//            memcpy(&mesh->vertices[triangle.indices[2]].normal, &normal, sizeof(v3d));
            
            // Smooth normals
            mesh->vertices[triangle.indices[0]].normal += normal;
            mesh->vertices[triangle.indices[1]].normal += normal;
            mesh->vertices[triangle.indices[2]].normal += normal;
            
            v3d tangent;
            v2d uv1, uv2;
            
            float c;
            
            uv1 = mesh->vertices[triangle.indices[1]].uv - mesh->vertices[triangle.indices[0]].uv;
            uv2 = mesh->vertices[triangle.indices[2]].uv - mesh->vertices[triangle.indices[0]].uv;
            
            c = 1.0f / (uv1.x * uv2.y - uv2.x * uv1.y);
            tangent.x = (v1.x * uv2.y + v2.x * uv1.y) * c;
            tangent.y = (v1.y * uv2.y + v2.y * uv1.y) * c;
            tangent.z = (v1.z * uv2.y + v2.z * uv1.y) * c;
            
            mesh->vertices[triangle.indices[0]].tangent += tangent;
            mesh->vertices[triangle.indices[1]].tangent += tangent;
            mesh->vertices[triangle.indices[2]].tangent += tangent;
            
            
        }
    
        for(auto &vertex : mesh->vertices){
            vertex.normal = vertex.normal.normalize();
            vertex.tangent = vertex.tangent.normalize();
        }
        
        for(unsigned int i = 0; i < mesh->nVertex; ++i){
            memset(&mesh->weights[i].normal, 0, sizeof(v3d));
            memset(&mesh->weights[i].tangent, 0, sizeof(v3d));
        }
        
        for(auto &vertex : mesh->vertices){
            for(int i = 0; i < vertex.count; ++i){
                Weight *md5weight = &mesh->weights[vertex.start + i];
                Joint md5joint = mBindPose[md5weight->joint];
                v3d normal(vertex.normal.x, vertex.normal.y, vertex.normal.z);
                v3d tangent(vertex.tangent.x, vertex.tangent.y, vertex.tangent.z);
                q4d rotation(md5joint.rotation.conjugate());
                normal = normal * rotation.matrix();
                tangent = tangent * rotation.matrix();
                md5weight->normal += normal;
                md5weight->tangent += tangent;
            }
        }
        
        for(unsigned int i = 0; i < mesh->weights.size(); ++i){
            mesh->weights[i].normal = mesh->weights[i].normal.normalize();
            mesh->weights[i].tangent = mesh->weights[i].tangent.normalize();
        }
    }
}

void MD5::updateBoundMesh(){
    min.x = min.y = min.z = 99999.999f;
    max.x = max.y = max.z = -99999.999f;
    
    for(auto &mesh : meshes){
        v3d *vertexArray = (v3d*)&mesh->vertexData[0];
        for(unsigned int i = 0; i < mesh->nVertex; ++i){
            if(vertexArray[i].x < min.x) min.x = vertexArray[i].x;
            if(vertexArray[i].y < min.y) min.y = vertexArray[i].y;
            if(vertexArray[i].z < min.z) min.z = vertexArray[i].z;
            
            if(vertexArray[i].x > max.x) max.x = vertexArray[i].x;
            if(vertexArray[i].y > max.y) max.y = vertexArray[i].y;
            if(vertexArray[i].z > max.z) max.z = vertexArray[i].z;
        }
    }
    
    dimension = (max - min);
    radius = v3d::length(max - min) * 0.5f;
}

void MD5::freeMeshData(){
    for(auto &mesh : meshes){
        mesh->indices.clear();
        mesh->triangles.clear();
    }
}


#pragma MD5Mesh related

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
    size = nVertex * (3 * sizeOfV3D + sizeOfV2D) ;
    vertexData.resize(size);
    offset[0] = 0;
    offset[1] = nVertex * sizeOfV3D;
    offset[2] = offset[1] + nVertex * sizeOfV3D;
    offset[3] = offset[2] + nVertex * sizeOfV2D;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, &vertexData[0], GL_DYNAMIC_DRAW);
    glGenBuffers(1, &vboIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
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

void Mesh::initMaterial(){
    material = Materials::instance()->getMaterial(shader);
    material->loadTextures();
    material->program = Materials::instance()->getProgramFoMesh(shader);
}

void Mesh::draw(){
    if(visible){
        if(material) material->use();
        if(vao) glBindVertexArray(vao);
        else setAttributes();
        
        glDrawElements(mode, nIndices, GL_UNSIGNED_SHORT, nullptr);
    }
}

void MD5::updateActions(const float timeStep){
    for(auto const& action : actions){
        if(action.second->state == Action::State::PLAY)
            updateAction(action.second, timeStep);
    }
}

bool MD5::updateAction(const sp<Action> action, const float timeStep){
    if(!action) return false;
    
    if(action->state == Action::State::PLAY){
        action->frameTime += timeStep;
        switch (action->method) {
            case Action::InterpolationMethod::FRAME:{
                if(action->frameTime >= action->fps){
                    action->pose = action->frame[action->currFrame];
                    ++action->currFrame;
                    if(action->currFrame == action->nFrames){
                        if(action->loop){
                            action->currFrame = 0;
                        }else{
                            action->state = Action::State::STOP;
                            currentActions.pop_back();
                            break;
                        }
                    }
                    action->nextFrame = action->currFrame + 1;
                    if(action->nextFrame == action->nFrames) action->nextFrame = 0;
                    action->frameTime -= action->fps;
                    return true;
                }
                break;
            }
            case Action::InterpolationMethod::LERP:
            case Action::InterpolationMethod::SLERP:{
                float t = std::max(0.0f, std::min((action->frameTime / action->fps) , 1.0f) );
                // blend pose goes here
                return true;
                break;
            }
            default:
                return false;
                break;
        }
    }
    return false;
}

void MD5::playAction(const string name, const Action::InterpolationMethod method, bool loop){
    
    sp<Action> action = getAction(name);
    if(!action) return;
    action->fps = 1.0f / 24.0f;
    action->method = method;
    action->loop = loop;
    action->currFrame = 0;
    action->state = md5::Action::State::PLAY;
    if(!action->frameTime && method == Action::InterpolationMethod::FRAME)
        action->frameTime = action->fps;

    
    if(currentActions.size() > 0){
        prevAction = currentActions.front();
    }
    
    currentActions.push_back(action);

//    logMessage("Current action: %s\n", currentActions[0]->name.c_str());
}

void MD5::stopAllActions(){
    for(auto const &act : actions){
        act.second->state = Action::State::STOP;
    }
}

void MD5::blendActions(const std::vector<Joint> &pose_1, const std::vector<Joint> &pose_2, Action::InterpolationMethod interpolationMethod, float blend){
    for(unsigned int i = 0; i < numJoints; ++i){
        mBindPose[i].location = v3d::lerp(pose_1[i].location, pose_2[i].location, blend);
        switch (interpolationMethod) {
            case Action::InterpolationMethod::FRAME:
            case Action::InterpolationMethod::LERP:
            case Action::InterpolationMethod::SLERP:
                mBindPose[i].rotation = q4d::lerp(pose_1[i].rotation, pose_2[i].rotation, blend);
                break;
            default:
                break;
        }
    }
}

void MD5::addActions(sp<Action> action1, sp<Action> action2, Action::InterpolationMethod interpolationMethod, float blend){
    for(unsigned int i = 0; i < numJoints; ++i){
        if((action1->frame[action1->currFrame][i].location == action1->frame[action1->nextFrame][i].location) || (action1->frame[action1->currFrame][i].rotation == action1->frame[action1->nextFrame][i].rotation)){
            mBindPose[i].location = v3d::lerp( action1->pose[i].location, action2->pose[i].location, blend);
            switch (interpolationMethod) {
                case Action::FRAME:
                case Action::LERP:
                case Action::SLERP:
                    mBindPose[i].rotation = q4d::lerp(action1->pose[i].rotation, action2->pose[i].rotation, blend);
                    break;
                default:
                    break;
            }
        }else{
            mBindPose[i].location = action2->pose[i].location;
            mBindPose[i].rotation = action2->pose[i].rotation;
        }

    }
}