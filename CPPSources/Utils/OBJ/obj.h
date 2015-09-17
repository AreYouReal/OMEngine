#pragma once

#include "SRUtils.h"
#include "m4d.h"
#include "ObjMesh.h"
#include "ObjMaterial.h"
#include "ShaderLibrary.h"
#include "Texture.h"

#include "NvTriStrip.h"

enum RenderObjectType{ SOLID, ALPHA_TESTED, TRANSPARENT };

struct Obj{

// Cons(des)tructor
    Obj(const char* filename);
    
// Mesh related
    void buildMesh(unsigned int meshIndex);
    void optimizeMesh(unsigned int meshIndex, unsigned int vertexCacheSize);
    
    unsigned int drawMesh(unsigned int meshIndex);
     
// Material related
    void buildMaterial(unsigned int matIndex, std::shared_ptr<ShaderProgram> program);
    
    unsigned int materialsSize(){ return (unsigned int)materials.size();  }
    unsigned int meshesSize()   { return (unsigned int)meshes.size();     }
    unsigned int texturesSize() { return (unsigned int)textures.size();   }
    void         generateTextureID(unsigned int textureIndex, unsigned int flags, unsigned int filter){
        textures[textureIndex]->generateID(flags, filter);
    }
    void        SetMaterialProgram(unsigned int matIndex, BindAttribCallback bindCallback){
        materials[matIndex]->program = ShaderLibrary::instance()->getProgram("defaultPerVertex");
        materials[matIndex]->program->bindAttribCallback = bindCallback;
    }
    
    void        SetMaterialCallback(unsigned int matIndex, DrawCallback callback){
        materials[matIndex]->materialDrawCalback = callback;
    }
    
    RenderObjectType renderObjectType(unsigned int meshIndex){
        std::shared_ptr<ObjMesh> mesh = meshes[meshIndex];
        if(mesh->currentMaterial){
            float dissolve = mesh->currentMaterial->dissolve;
            if(dissolve == 0.0f){
                return ALPHA_TESTED;
            }else if(dissolve == 1.0f){
                return SOLID;
            }else return TRANSPARENT;
        }
        return SOLID;
    }
    

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
    
    static void readIndices(const char* line, int v[], int n[], int uv[], bool &useUVs);
    
// Helpers
    void    loadMaterial(const char* filename);
    void    addTexture(const char* filename);
    int     getTextureIndex(const char* filename);
    std::shared_ptr<ObjMaterial> getMaterial(const char* name);
    
// Mesh related
    void updateBoundMesh(unsigned int meshIndex);
    void updateMin(v3d &min, v3d &vertex);
    void updateMax(v3d &max, v3d &vertex);
    void buildVBOMesh(unsigned int meshIndex);
};
