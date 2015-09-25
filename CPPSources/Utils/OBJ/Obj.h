#pragma once

#include "SRUtils.h"
#include "m4d.h"
#include "ObjMesh.h"
#include "ObjMaterial.h"
#include "ShaderLibrary.h"
#include "Texture.h"
#include "Materials.hpp"


#include "NvTriStrip.h"



struct ObjTriangleList;
class ObjMesh;



class Obj{
    friend class ObjMesh;
public:
    // Cons(des)tructor
    Obj(const char* filename);

    void optimizeMesh(unsigned int meshIndex, unsigned int vertexCacheSize);
    unsigned int drawMesh(unsigned int meshIndex);
    
    std::shared_ptr<ObjMesh> getMesh(unsigned int index);
     
// Material related
    void generateTextureID(unsigned int textureIndex, unsigned int flags, unsigned int filter);
    void SetMaterialProgram(unsigned int matIndex);
    
    unsigned int materialsSize();
    unsigned int meshesSize();
    unsigned int texturesSize();
    
// Fields
private:
    std::string     texturePath;    // The texture path (relative to the .mtl file)
    std::string     programPath;    // The shader program path ( relative to the .mtl file)
    
    std::vector<std::shared_ptr<ObjMesh>> meshes;
    
    std::vector<std::shared_ptr<ObjMaterial>>    materials;
    std::vector<std::shared_ptr<Texture>>        textures;
    std::vector<std::shared_ptr<ShaderProgram>>  programs;
    
    std::vector<v3d>            vertices;
    std::vector<v3d>            normals;
    std::vector<v3d>            faceNormals;
    std::vector<v3d>            tangents;
    std::vector<v3d>            UVs;
    
// Static
    void addMesh(std::shared_ptr<ObjMesh> &mesh, std::shared_ptr<ObjTriangleList> &tList, char* name, char* usemtl, bool useUVs);
    void builNormalsAndTangents();
    
    static bool readIndices(const char* line, int v[], int n[], int uv[], bool &useUVs);
    
// Mesh related
    void updateBoundMesh(unsigned int meshIndex);
    void updateMin(v3d &min, v3d &vertex);
    void updateMax(v3d &max, v3d &vertex);
};

typedef std::shared_ptr<Obj> OBJ;
