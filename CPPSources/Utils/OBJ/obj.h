#pragma once

#include "SRUtils.h"
#include "m4d.h"
#include "ObjMesh.h"
#include "ObjMaterial.h"
#include "ShaderHelper.h"
#include "Texture.h"

struct Obj{

// Cons(des)tructor
    Obj(const char* filename);
    
// Mesh related
    void buildMesh(unsigned int meshIndex);
    unsigned int drawMesh(unsigned int meshIndex);
    
// Material related
    void buildMaterial(unsigned int matIndex, std::shared_ptr<ShaderProgram> program);
    void drawMaterial(ObjMaterial *mat);
    
    unsigned int materialsSize(){ return materials.size();  }
    unsigned int meshesSize()   { return meshes.size();     }
    unsigned int texturesSize() { return textures.size();   }
    void         generateTextureID(unsigned int textureIndex, unsigned int flags, unsigned int filter){
        textures[textureIndex].generateID(flags, filter);
    }
    void        SetMaterialProgram(unsigned int matIndex, std::shared_ptr<ShaderProgram> program){
        materials[matIndex].program = program;
    }
    
    void        SetMaterialCallback(unsigned int matIndex, DrawCallback callback){
        materials[matIndex].materialDrawCalback = callback;
    }
    

// Fields
private:
    std::string     texturePath;    // The texture path (relative to the .mtl file)
    std::string     programPath;    // The shader program path ( relative to the .mtl file)
    
    std::vector<ObjMesh> meshes;
    
    std::vector<ObjMaterial>    materials;
    std::vector<Texture>        textures;
    std::vector<ShaderProgram>  programs;
    
    std::vector<v3d>            vertices;
    std::vector<v3d>            normals;
    std::vector<v3d>            faceNormals;
    std::vector<v3d>            tangents;
    std::vector<v3d>            UVs;


    
// Static
    void addMesh(ObjMesh **mesh, ObjTriangleList **tList, char* name, char* usemtl, bool useUVs);
    void builNormalsAndTangents();
    
    static void readIndices(const char* line, int v[], int n[], int uv[], bool &useUVs);
    
// Helpers
    void    loadMaterial(const char* filename);
    void    addTexture(const char* filename);
    int     getTextureIndex(const char* filename);
    ObjMaterial *getMaterial(const char* name);
    
// Mesh related
    void updateBoundMesh(unsigned int meshIndex);
    void updateMin(v3d &min, v3d &vertex);
    void updateMax(v3d &max, v3d &vertex);
    void buildVBOMesh(unsigned int meshIndex);
};
