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
class   ObjMeshData;

typedef std::shared_ptr<ObjMeshData> OBJMESHDATA;
class ObjMeshData{
    friend class ObjMesh;
public:
    // Constructor
    static OBJMESHDATA load(const char* filename);

    void        optimizeMesh(unsigned int meshIndex, unsigned int vertexCacheSize);
    unsigned int drawMesh(unsigned int meshIndex);
    
    std::shared_ptr<ObjMesh> getMesh(unsigned int index);
    unsigned int meshesSize();
    
// Fields
private:
    ObjMeshData();
    
    std::vector<std::shared_ptr<ObjMesh>> meshes;
    std::vector<v3d>            vertices;
    std::vector<v3d>            normals;
    std::vector<v3d>            faceNormals;
    std::vector<v3d>            tangents;
    std::vector<v3d>            UVs;
    
// Static
    void addMesh(std::shared_ptr<ObjMesh> &mesh, std::shared_ptr<ObjTriangleList> &tList, char* name, char* usemtl, char* group, bool useUVs);
    void builNormalsAndTangents();
    static bool readIndices(const char* line, int v[], int n[], int uv[], bool &useUVs);
    
// Mesh related
};




