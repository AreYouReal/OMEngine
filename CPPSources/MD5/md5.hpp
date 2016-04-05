#pragma once

#include "OMUtils.h"
#include "ObjMaterial.h"

struct md5joint{
    string  name;
    int     parent;
    v3d     location;
    v4d     rotation;
};

struct md5vertex{
    v2d     uv;
    v3d     normal;
    v3d     tangent;
    unsigned int start;
    unsigned int count;
};

struct md5triangle{
    unsigned short indices[3];
};

struct md5weight{
    int     joint;
    float   bias;
    v3d     location;
    v3d     normal;
    v3d     tangent;
};


struct md5mesh{
    string name;
    unsigned int nVertex;
    std::vector<md5vertex>  vertices;
    unsigned int vbo;
    unsigned int size;
    unsigned int stride;
    unsigned int offset[4];
    unsigned char   *vertexData;
    unsigned int    nTriangle;
    std::vector<md5triangle>    triangles;
    unsigned int mode;
    unsigned short nIndice;
    unsigned short *indices;
    unsigned int vboIndice;
    unsigned int nWeight;
    std::vector<md5weight> weights;
    unsigned int vao;
    bool visible;
    sp<ObjMaterial> material;
};

struct md5action{
    string name;
    unsigned int nFrame;
    md5joint    **frame;    // ????
    md5joint    *pose;
    int         currFrame;
    int         nextFrame;
    unsigned char state;
    unsigned char method;
    unsigned char loop;
    float frameTime;
    float fps;
};

struct md5{
    string name;
    bool    visible;
    unsigned int nJoint;
    md5joint    *bindPose;
    unsigned int nMesh;
    std::vector<md5mesh> meshes;
    unsigned int nAction;
    std::vector<md5action>  actions;
    v3d     location;
    v3d     rotation;
    v3d     scale;
    v3d     min;
    v3d     max;
    v3d     dimension;
    float radius;
    float distance;
    btRigidBody *btrigidbody;
};
