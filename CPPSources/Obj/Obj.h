#pragma once

#include "SRUtils.h"
#include "m4d.h"
#include "ObjMesh.h"
#include "ObjMaterial.h"
#include "ShaderLibrary.h"
#include "Texture.h"
#include "Materials.hpp"


#include "NvTriStrip.h"

struct  ObjTriangleList;
class   ObjMesh;
class   Obj;

class Obj : public std::enable_shared_from_this<Obj>{
    friend class ObjMesh;
public:
    ~Obj();
    
    static sp<Obj> load(const char* filename);
    
    void build();
    
    void clear();

    void        optimizeMesh(str meshName, unsigned int vertexCacheSize);
    unsigned int drawMesh(unsigned int meshIndex);
    
    sp<ObjMesh>  getMesh(str meshName);
    unsigned int meshesSize();
    
// Fields
private:
    Obj();
    
    std::map<str, sp<ObjMesh>>  meshes;
    std::vector<v3d>            vertices;
    std::vector<v3d>            normals;
    std::vector<v3d>            faceNormals;
    std::vector<v3d>            tangents;
    std::vector<v3d>            UVs;
    
// Static
    void addMesh(sp<ObjMesh> mesh, sp<ObjTriangleList> tList, char* name, char* usemtl, char* group, bool useUVs);
    void builNormalsAndTangents();
    static bool readIndices(const char* line, int v[], int n[], int uv[], bool &useUVs);
    
// Mesh related
};




