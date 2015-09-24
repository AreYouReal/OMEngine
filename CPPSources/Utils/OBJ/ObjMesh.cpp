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
