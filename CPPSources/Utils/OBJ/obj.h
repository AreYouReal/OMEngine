#pragma once

#include "SRUtils.h"
#include "q4d.h"

// Object material structure definition
struct ObjMaterial{
    char    name[MAX_CHAR];                 // The material name!
    v4d     ambient;                        // Ambient material color.
    v4d     diffuse;                        // Diffuse material color.
    v4d     specular;                       // Specular material color.
    
    float   specularExponent;               // Specular exponent (aka Hardness or Shiness).
    
};

// Hold all the vertex and UV indices for the particular triangle.
struct ObjTriangleIndex{
    int vertexIndex[3];
    int uvIndex[3];
};

// Represent the one triangle list onside ObjMesh
struct ObjTriangleList{
    unsigned int        nObjTriangleIndex;  // The number of triangle index.
    ObjTriangleIndex    *objTriangleIndex;  // Triangle array that contain each triangle index data.
    bool                useUVs;             // True if triangle list is using UVs.
    unsigned short      nIndiceArray;       // The number of indice required to draw the triangle list.
    unsigned short      *indiceArray;       // Array of indices.
    ObjMaterial         *objMaterial;       // POinter to the material to use when draw this list.
    int                 mode;               // Drawing mode (Default: GL_TRIANGLES).
    unsigned int        vbo;
};

// Holds the unique vertex data index
struct ObjVertexData{
    int vertexIndex;
    int uvIndex;
};

struct ObjMesh{
    char            name[MAX_CHAR];         // Mesh name.
    bool            visible;                // If true - it's visible.
    unsigned short  nObjVertexData;         // The number of ObjVertexData.
    ObjVertexData   *objVertexData;         // Well, ObjVertexData themselves.
    unsigned char   nTrinagleList;          // The number of triangle lists for this ObjMesh.
    ObjTriangleList *objTriangleList;       // Array og ObjTriangleList - basically at least one need to draw the ObjMesh.
    ObjMaterial          *currentMaterial;       // Current object material.
    
    v3d             location;
    v3d             rotation;
    v3d             scale;
    
    unsigned int    vbo;                    // The vertex buffer VBO ID maintaned by GLES.
    unsigned int    stride;                 // Stride size in bytes to determine next data chunk location.
    unsigned int    size;                   // Total size of the vertex data array.
    unsigned int    offset[ 5 ];            // The VBO offset(????)
    unsigned int    vao;                    // The VAO ID maintaned by GLES
    
    
};

struct Obj{
    char            texturePath[ MAX_CHAR ];// The texture path (relative to the .mtl file)
    char            programPath[ MAX_CHAR ];// The shader program path ( relative to the .mtl file)
    unsigned int    nObjMesh;               // The number of ObjMesh
    ObjMesh         *objMesh;               // Array of ObjMesh for each objec entry of the .obj file
    unsigned int    nObjectMaterial;        // The number of ObjMaterial
    ObjMaterial     *objMaterial;           // The array of ObjMaterial

    // Textures goes here
    
    unsigned int    n_program;              // The number of shader program the Obj is using.
    
    
};
