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
    std::vector<unsigned char>   vertexData;
    unsigned int    nTriangle;
    std::vector<Triangle>    triangles;
    unsigned int mode = GL_TRIANGLES;
    unsigned short nIndices;
    std::vector<unsigned short> indices;
    unsigned int vboIndices;
    unsigned int nWeight;
    std::vector<Weight> weights;
    unsigned int vao;
    bool visible = true;
    sp<ObjMaterial> material;
    
    void buildVBO();
    void buildVAO();
    void setAttributes();
    
    void initMaterial();
    
    void draw();
};

struct Action{
    string name;
    unsigned int nFrames;
    std::vector<std::vector<Joint>>    frame;    // ????
    std::vector<Joint>    pose;
    int         currFrame;
    int         nextFrame;
    unsigned char state;
    unsigned char method;
    bool    loop;
    float frameTime;
    float fps;
    
    
    enum InterpolationMethod {
        FRAME = 0,
        LERP  = 1,
        SLERP = 2
    };
    
    enum State
    {
        STOP  = 0,
        PLAY  = 1,
        PAUSE = 2
    };
};
    
class MD5{
public:
        enum AnimType{ SINGLE_ACTION, BLEND_ACTIONS, ADD_ACTIONS };
    
        string name;
        bool    visible;
        AnimType mAnimType = SINGLE_ACTION;
    
        unsigned int numJoints;
        std::vector<Joint> mBindPose;
        unsigned int numMeshes;
        std::vector<sp<Mesh>> meshes;
        unsigned int nActions;
        std::map<string, sp<Action>>  actions;
        v3d     location;
        v3d     rotation;
        v3d     scale;
        v3d     min;
        v3d     max;
        v3d     dimension;
        float radius;
        float distance;
        
        void optimize(unsigned int vertexCacheSize);
        void build();
        
        void draw();
        
        void freeMeshData();
        
        static sp<MD5> loadMesh(const string filename);
        
        sp<Action> loadAction(const string name, const string filename);
        sp<Action> getAction(const string name);
        
        bool updateAction(const sp<Action> action, const float timeStep);
        void updateActions(const float timeStep);
        
        void playAction(const string name, const Action::InterpolationMethod method);
        
        void blendActions(const std::vector<Joint> &pose_1, const std::vector<Joint> &pose_2, Action::InterpolationMethod interpolationMethod, float blend);
    
    void addActions(sp<Action> action1, sp<Action> action2, Action::InterpolationMethod interpolationMethod, float blend);
        
    private:
        static sp<Mesh> loadMeshData(char* line);
        
    std::vector<sp<Action>> currentActions;

    
        
        void buildPoseWeightedNormalsTangents();
        void updateBoundMesh();
        void setPose();
};
    
}




