#include "ObjMesh.h"
#include "Scene.hpp"

ObjMesh::ObjMesh(){
    logMessage("ObjMesh constructor! %s \n" , name.c_str());
}

ObjMesh::~ObjMesh(){
    logMessage("ObjMesh destructor! %s \n" , name.c_str());
}

unsigned int ObjMesh::draw(){
    unsigned int n = 0;
    
    if(!visible) return n;
    
    Scene::instance()->setRenderObjectState(renderObjectType());
    
    if(glInfo.vao) glBindVertexArray(glInfo.vao);
    else setAttributes();
    
    for(unsigned int i = 0; i < tLists.size(); ++i){
        currentMaterial = tLists[i]->material;
        if(currentMaterial) currentMaterial->use();
        if(glInfo.vao){
            if(tLists.size() != 1) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tLists[i]->vbo);
        }else{
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tLists[i]->vbo);
        }
        glDrawElements(tLists[i]->mode, (unsigned int)tLists[i]->indices.size(), GL_UNSIGNED_SHORT, (void*)NULL);
        n += tLists[i]->indices.size();
    }
    return n;
}

void ObjMesh::setAttributes(){
    glBindBuffer(GL_ARRAY_BUFFER, glInfo.vbo);
    
    unsigned char size = sizeof(v3d)/sizeof(float);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, glInfo.stride, (char *)NULL);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, size, GL_FLOAT, GL_FALSE, glInfo.stride, (char *)NULL + glInfo.offset[1]);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, size, GL_FLOAT, GL_FALSE, glInfo.stride, (char *)NULL + glInfo.offset[2]);
    if(glInfo.offset[3] != -1){
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, size, GL_FLOAT, GL_FALSE, glInfo.stride, (char *)NULL + glInfo.offset[3]);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, size, GL_FLOAT, GL_FALSE, glInfo.stride, (char *)NULL + glInfo.offset[4]);
    }
}

RenderObjectType ObjMesh::renderObjectType(){
    if(currentMaterial){
        float dissolve = currentMaterial->dissolve;
        if(dissolve == 0.0f){
            return RenderObjectType::ALPHA_TESTED;
        }else if(dissolve == 1.0f){
            return RenderObjectType::SOLID;
        }else return RenderObjectType::TRANSPARENT;
    }
    return RenderObjectType::SOLID;
}

void ObjMesh::build(){
    updateBounds();
    buildVBO();
    initMaterial();

    glGenVertexArrays(1, &glInfo.vao);
    glBindVertexArray(glInfo.vao);
    
    setAttributes();
    
    if(tLists.size() == 1){ glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tLists[0]->vbo); }
    
    glBindVertexArray(0);
}

void ObjMesh::updateBounds(){
    outlines.min.x = outlines.min.y = outlines.min.z = 9999.99f;
    outlines.max.x = outlines.max.y = outlines.max.z = -9999.99f;
    
    unsigned int index;
    auto data = weakData.lock();
    if(!data){
        logMessage("Unable to reach the data for mesh!");
        return;
    }
    for(unsigned int i = 0; i < vertexData.size(); ++i){
        index = vertexData[i].vIndex;
        updateMin(outlines.min, data->vertices[index]);
        updateMax(outlines.max, data->vertices[index]);
    }
    
    outlines.location = (outlines.min + outlines.max) * 0.5f;
    outlines.dimension = outlines.max - outlines.min;
    //    mesh->radius =  mesh->dimension.x >= mesh->dimension.y ?
    //                    mesh->dimension.x :
    //                    mesh->dimension.y;
    //    mesh->radius =  mesh->radius >= mesh->dimension.x ?
    //                    mesh->radius * 0.5f :
    //                    mesh->dimension.z * 0.5f;
    outlines.radius = v3d::length(outlines.max - outlines.min) * 0.5f;
}

void ObjMesh::updateMax(v3d &max, v3d &vertex){
    if(vertex.x > max.x) max.x = vertex.x;
    if(vertex.y > max.y) max.y = vertex.y;
    if(vertex.z > max.z) max.z = vertex.z;
}

void ObjMesh::updateMin(v3d &min, v3d &vertex){
    if(vertex.x < min.x) min.x = vertex.x;
    if(vertex.y < min.y) min.y = vertex.y;
    if(vertex.z < min.z) min.z = vertex.z;
}

void ObjMesh::buildVBO(){
    unsigned int v3dSize = sizeof(v3d);
    glInfo.stride = v3dSize;
    glInfo.stride += v3dSize;
    glInfo.stride += v3dSize;
    
    if(vertexData[0].uvIndex != -1){
        glInfo.stride += v3dSize;
        glInfo.stride += v3dSize;
    }
    
    glInfo.size = (unsigned int)vertexData.size() * glInfo.stride;
    
    unsigned char *vertexArray = (unsigned char*) malloc(glInfo.size);
    unsigned char *vertexStart = vertexArray;
    
    unsigned int index;
    auto data = weakData.lock();
    if(!data){
        logMessage("Unable to reach the data for mesh!");
        return;
    }
    for(unsigned int i = 0; i < vertexData.size(); ++i){
        index = vertexData[i].vIndex;
        memcpy(vertexArray, &data->vertices[index], v3dSize);
        // Center the pivot
        v3d centerThePivot = data->vertices[index] - outlines.location;      // ??????????
        memcpy(vertexArray, &centerThePivot, v3dSize);                       // ??????????
        vertexArray += v3dSize;
        memcpy(vertexArray, &data->normals[index], v3dSize);
        vertexArray += v3dSize;
        memcpy(vertexArray, &data->faceNormals[index], v3dSize);
        vertexArray += v3dSize;
        if(vertexData[0].uvIndex != -1){
            memcpy(vertexArray, &data->UVs[vertexData[i].uvIndex], v3dSize);
            vertexArray += v3dSize;
            memcpy(vertexArray, &data->tangents[index], v3dSize);
            vertexArray += v3dSize;
        }
    }
    
    glGenBuffers(1, &glInfo.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, glInfo.vbo);
    glBufferData(GL_ARRAY_BUFFER, glInfo.size, vertexStart, GL_STATIC_DRAW);
    free(vertexStart);
    
    //    logMessage("Mesh vertices vbo:  ARRAY  %d\n", mesh->vbo);
    
    unsigned int off = 0;
    glInfo.offset[0] = off;
    off += v3dSize;
    glInfo.offset[1] = off;
    off += v3dSize;
    glInfo.offset[2] = off;
    if(vertexData[0].uvIndex != -1){
        off += v3dSize;
        glInfo.offset[3] = off;
        off += v3dSize;
        glInfo.offset[4] = off;
    }
    
    for(unsigned int i = 0; i < tLists.size(); ++i){
        glGenBuffers(1, &tLists[i]->vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tLists[i]->vbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, tLists[i]->indices.size() * sizeof(unsigned short), &tLists[i]->indices[0], GL_STATIC_DRAW);
        //        logMessage("tList VBO: %d\n", mesh->tLists[i].vbo);
    }
}

void ObjMesh::initMaterial(){
    for(unsigned int i = 0; i < tLists.size(); ++i){
        if(tLists[i]->material != nullptr){
            tLists[i]->material->loadTextures();
            tLists[i]->material->setProgram(Materials::instance()->getProgram("norm_as_color"));
        }
    }
}

void ObjMesh::clear(){
    vertexData.clear();
}

