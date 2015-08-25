#pragma once

#include "SRUtils.h"
#include "m4d.h"
#include "ShaderHelper.h"

// Object material structure definition
struct ObjMaterial{
    char    name[MAX_CHAR];                 // The material name!
    v4d     ambient;                        // Ambient material color.
    v4d     diffuse;                        // Diffuse material color.
    v4d     specular;                       // Specular material color.
    float   specularExponent;               // Specular exponent (aka Hardness or Shiness).
    float   dissolve;                       // The material dissolve factor a.k.a alpha
    char    mapAmbient[MAX_CHAR];           // The ambient texture channel filename
    char    mapDiffuse[MAX_CHAR];           // The diffuse texture channel filename
    char    mapSpecular[MAX_CHAR];          // The specular texture channel filename
    char    mapTranslucency[MAX_CHAR];      // The translucecny....
    char    mapDisp[MAX_CHAR];              // The displacement....
    char    mapBump[MAX_CHAR];              // The bump map(aka Normal Map)
    
};

// Hold all the vertex and UV indices for the particular triangle.
struct ObjTriangleIndex{ int vertexIndex[3], uvIndex[3]; };

// Represent the one triangle list onside ObjMesh
struct ObjTriangleList{
    unsigned int        nTIndex;  // The number of triangle index.
    ObjTriangleIndex    *tIndex;  // Triangle array that contain each triangle index data.
    unsigned short      nIndices;  // The number of indice required to draw the triangle list.
    unsigned short      *indices;  // Array of indices.
    ObjMaterial         *material;  // Pointer to the material to use when draw this list.
    bool                useUVs;           // True if triangle list is using UVs.
    int                 mode;             // Drawing mode (Default: GL_TRIANGLES).
    unsigned int        vbo;
};

// Holds the unique vertex data index
struct ObjVertexData{ int vIndex, uvIndex; };

struct ObjMesh{
    char            name[MAX_CHAR];  // Mesh name.
    bool            visible;         // If true - it's visible.
    unsigned short  nObjVertexData;  // The number of ObjVertexData.
    ObjVertexData   *objVertexData;  // Well, ObjVertexData themselves.
    unsigned char   nTList;          // The number of triangle lists for this ObjMesh.
    ObjTriangleList *tList  = NULL;          // Array og ObjTriangleList - basically at least one need to draw the ObjMesh.
    ObjMaterial     *material;       // Current object material.
    
    unsigned int    vbo;             // The vertex buffer VBO ID maintaned by GLES.
    unsigned int    stride;          // Stride size in bytes to determine next data chunk location.
    unsigned int    size;            // Total size of the vertex data array.
    unsigned int    offset[ 5 ];     // The VBO offset(????)
    unsigned int    vao;             // The VAO ID maintaned by GLES
    
    void            addVertexData(ObjTriangleList *otl, int vIndex, int uvIndex);
    void            addIndexToTriangleList(ObjTriangleList *otl, int index);
};

struct Obj{
    static Obj* load(const char* filename);
    
    void loadMaterial(const char* filename);
    
    char            texturePath[ MAX_CHAR ];// The texture path (relative to the .mtl file)
    char            programPath[ MAX_CHAR ];// The shader program path ( relative to the .mtl file)
    unsigned int    nObjMesh;               // The number of ObjMesh
    ObjMesh         *objMesh;               // Array of ObjMesh for each objec entry of the .obj file
    unsigned int    nMaterials;             // The number of ObjMaterial
    ObjMaterial     *materials;             // The array of ObjMaterial

    // Textures goes here
    
    unsigned int    n_program;              // The number of shader program the Obj is using.
    ShaderProgram   *program;
    
    // Vertices data goes here
    unsigned int    nVertices;
    v3d             *vertices;
    v3d             *normals;
    v3d             *faceNormals;
    v3d             *tangents;
    
    unsigned int    nUVs;
    v3d             *UVs;
};
