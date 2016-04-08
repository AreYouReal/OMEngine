#pragma once

#include "OMUtils.h"
#include "ObjMaterial.h"

namespace md5{
struct Joint{
    string  name;
    int     parent;
    v3d     location;
    q4d     rotation;
};

struct Vertex{
    v2d     uv;
    v3d     normal;
    v3d     tangent;
    unsigned int start;
    unsigned int count;
};

struct Triangle{
    unsigned short indices[3];
};

struct Weight{
    int     joint;
    float   bias;
    v3d     location;
    v3d     normal;
    v3d     tangent;
};

struct Mesh{
    string shader;
    unsigned int nVertex;
    std::vector<Vertex>  vertices;
    unsigned int vbo;
    unsigned int size;
    unsigned int stride;
    unsigned int offset[4];
    unsigned char   *vertexData;
    unsigned int    nTriangle;
    std::vector<Triangle>    triangles;
    unsigned int mode;
    unsigned short nIndice;
    std::vector<unsigned short> indices;
    unsigned int vboIndice;
    unsigned int nWeight;
    std::vector<Weight> weights;
    unsigned int vao;
    bool visible;
    sp<ObjMaterial> material;
};

struct Action{
    string name;
    unsigned int nFrame;
    Joint    **frame;    // ????
    Joint    *pose;
    int         currFrame;
    int         nextFrame;
    unsigned char state;
    unsigned char method;
    unsigned char loop;
    float frameTime;
    float fps;
};
    
    class MD5{
    public:
        
        string name;
        bool    visible;
        unsigned int numJoints;
        std::vector<sp<Joint>> bindPose;
        unsigned int numMeshes;
        std::vector<sp<Mesh>> meshes;
        unsigned int nAction;
        std::vector<Action>  actions;
        v3d     location;
        v3d     rotation;
        v3d     scale;
        v3d     min;
        v3d     max;
        v3d     dimension;
        float radius;
        float distance;
        btRigidBody *btrigidbody;
        
        void optimize(unsigned int vertexCacheSize);
        
        
        static sp<MD5> loadMesh(string filename);
        
    private:
        static sp<Mesh> loadMeshData(char* line);
        
    };
    
}




