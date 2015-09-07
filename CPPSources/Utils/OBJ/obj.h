#pragma once

#include "SRUtils.h"
#include "m4d.h"
#include "ShaderHelper.h"
#include "Texture.h"

// Object material structure definition
struct ObjMaterial{
    std::string name;                 // The material name!
    v4d         ambient;              // Ambient material color.
    v4d         diffuse;              // Diffuse material color.
    v4d         specular;             // Specular material color.
    v3d         transmissionFilter;   // Transmission filter of the material
    int         illuminationModel;    // ....
    float       specularExponent;     // Specular exponent (aka Hardness or Shiness).
    float       dissolve;             // The material dissolve factor a.k.a alpha
    float       opticalDensity;       // ...
    std::string mapAmbient;           // The ambient texture channel filename
    std::string mapDiffuse;           // The diffuse texture channel filename
    std::string mapSpecular;          // The specular texture channel filename
    std::string mapTranslucency;      // The translucecny....
    std::string mapDisp;              // The displacement....
    std::string mapBump;              // The bump map(aka Normal Map)
    
    Texture *tAmbient, *tDiffuse, *tSpecular, *tTranslucency, *tDisp, *tBump;
    ShaderProgram   *program;
    DrawCallback    *materialDrawCalback;
    
};

// Hold all the vertex and UV indices for the particular triangle.
struct ObjTriangleIndex{ int vertexIndex[3], uvIndex[3]; };

// Represent the one triangle list onside ObjMesh
struct ObjTriangleList{
    std::vector<ObjTriangleIndex>   tIndices;
    std::vector<unsigned short>     indices;
    ObjMaterial                     *material;  // Pointer to the material to use when draw this list.
    bool                            useUVs;     // True if triangle list is using UVs.
    int                             mode;       // Drawing mode (Default: GL_TRIANGLES).
    unsigned int                    vbo;
};

// Holds the unique vertex data index
struct ObjVertexData{ int vIndex, uvIndex; };

struct ObjMesh{
    
    // Fields
    std::string                     name;  // Mesh name.
    bool                            visible;         // If true - it's visible.
    std::vector<ObjVertexData>      vertexData;
    std::vector<ObjTriangleList>    tLists;
    ObjMaterial                     *material;       // Current object material.
    
    v3d                             min;
    v3d                             max;
    v3d                             location;
    v3d                             dimension;
    float                           radius;
    
    unsigned int                    vbo;             // The vertex buffer VBO ID maintaned by GLES.
    unsigned int                    stride;          // Stride size in bytes to determine next data chunk location.
    unsigned int                    size;            // Total size of the vertex data array in bytes.
    unsigned int                    offset[ 5 ];     // The VBO offset(????)
    unsigned int                    vao;             // The VAO ID maintaned by GLES

    // Helpers
    void            addVertexData(ObjTriangleList *otl, int vIndex, int uvIndex);
};

struct Obj{

// Cons(des)tructor
    Obj(const char* filename);
    
// Mesh related
    void buildMesh(unsigned int meshIndex);
    
    unsigned int drawMesh(unsigned int meshIndex);
    
// Material related
    void buildMaterial(unsigned int matIndex, ShaderProgram *program);

// Fields
    
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

private:
    
// Static
    void addMesh(ObjMesh **mesh, ObjTriangleList **tList, char* name, char* usemtl, bool useUVs);
    void builNormalsAndTangents();
    
    static void readIndices(const char* line, int v[], int n[], int uv[], bool &useUVs);
    
    
// Helpers
    void    loadMaterial(const char* filename);
    void    addTexture(const char* filename);
    int     getTextureIndex(const char* filename);
    
// Mesh related
    void updateBoundMesh(unsigned int meshIndex);
    void updateMin(v3d &min, v3d &vertex);
    void updateMax(v3d &max, v3d &vertex);
    
    void buildVBOMesh(unsigned int meshIndex);
    
    void setMeshAttributes(unsigned int meshIndex);
};
