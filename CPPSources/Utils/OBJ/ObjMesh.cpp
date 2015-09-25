#include "ObjMesh.h"


unsigned int ObjMesh::draw(){
    unsigned int n = 0;
    
    if(!visible) return n;
    
    if(vao) glBindVertexArray(vao);
    else setAttributes();
    
    for(unsigned int i = 0; i < tLists.size(); ++i){
        currentMaterial = tLists[i]->material;
        if(currentMaterial) currentMaterial->use();
        if(vao){
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
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    unsigned char size = sizeof(v3d)/sizeof(float);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, stride, (char *)NULL);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, size, GL_FLOAT, GL_FALSE, stride, (char *)NULL + offset[1]);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, size, GL_FLOAT, GL_FALSE, stride, (char *)NULL + offset[2]);
    if(offset[3] != -1){
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, size, GL_FLOAT, GL_FALSE, stride, (char *)NULL + offset[3]);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, size, GL_FLOAT, GL_FALSE, stride, (char *)NULL + offset[4]);
    }
}

RenderObjectType ObjMesh::renderObjectType(){
    if(currentMaterial){
        float dissolve = currentMaterial->dissolve;
        if(dissolve == 0.0f){
            return ALPHA_TESTED;
        }else if(dissolve == 1.0f){
            return SOLID;
        }else return TRANSPARENT;
    }
    return SOLID;
}

void ObjMesh::build(){
//    updateBoundMesh();
    buildVBO();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    setAttributes();
    
    if(tLists.size() == 1){ glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tLists[0]->vbo); }
    
    glBindVertexArray(0);
}

void ObjMesh::updateBounds(){
    min.x = min.y = min.z = 9999.99f;
    max.x = max.y = max.z = -9999.99f;
    
    unsigned int index;
    for(unsigned int i = 0; i < vertexData.size(); ++i){
        index = vertexData[i].vIndex;
        updateMin(min, data->vertices[index]);
        updateMax(max, data->vertices[index]);
    }
    
    location = (min + max) * 0.5f;
    dimension = max - min;
    //    mesh->radius =  mesh->dimension.x >= mesh->dimension.y ?
    //                    mesh->dimension.x :
    //                    mesh->dimension.y;
    //    mesh->radius =  mesh->radius >= mesh->dimension.x ?
    //                    mesh->radius * 0.5f :
    //                    mesh->dimension.z * 0.5f;
    radius = v3d::length(max - min) * 0.5f;
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
    stride = v3dSize;
    stride += v3dSize;
    stride += v3dSize;
    
    if(vertexData[0].uvIndex != -1){
        stride += v3dSize;
        stride += v3dSize;
    }
    
    size = (unsigned int)vertexData.size() * stride;
    
    unsigned char *vertexArray = (unsigned char*) malloc(size);
    unsigned char *vertexStart = vertexArray;
    
    unsigned int index;
    for(unsigned int i = 0; i < vertexData.size(); ++i){
        index = vertexData[i].vIndex;
        memcpy(vertexArray, &data->vertices[index], v3dSize);
        // Center the pivot
        //        v3d centerThePivot = vertices[index] - mesh->location;      // ??????????
        //        memcpy(vertexArray, &centerThePivot, v3dSize);              // ??????????
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
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, vertexStart, GL_STATIC_DRAW);
    free(vertexStart);
    
    //    logMessage("Mesh vertices vbo:  ARRAY  %d\n", mesh->vbo);
    
    unsigned int off = 0;
    offset[0] = off;
    off += v3dSize;
    offset[1] = off;
    off += v3dSize;
    offset[2] = off;
    if(vertexData[0].uvIndex != -1){
        off += v3dSize;
        offset[3] = off;
        off += v3dSize;
        offset[4] = off;
    }
    
    for(unsigned int i = 0; i < tLists.size(); ++i){
        glGenBuffers(1, &tLists[i]->vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tLists[i]->vbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, tLists[i]->indices.size() * sizeof(unsigned short), &tLists[i]->indices[0], GL_STATIC_DRAW);
        //        logMessage("tList VBO: %d\n", mesh->tLists[i].vbo);
    }
}


