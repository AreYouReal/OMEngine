#pragma once

#include "SRUtils.h"
#include "ObjMaterial.h"
#include "Obj.h"

enum class RenderObjectType{ SOLID, ALPHA_TESTED, TRANSPARENT };

// Hold all the vertex and UV indices for the particular triangle.
struct ObjTriangleIndex{ int vertexIndex[3], uvIndex[3]; };

// Represent the one triangle list onside ObjMesh
struct ObjTriangleList{
    ObjTriangleList(){logMessage("ObjTriangleList Constructor!\n");}
    ~ObjTriangleList(){logMessage("ObjTriangleList Destructor!\n"); }
    std::vector<ObjTriangleIndex>   tIndices;
    std::vector<unsigned short>     indices;
    sp<ObjMaterial>                 material;  // Pointer to the material to use when draw this list.
    bool                            useUVs;     // True if triangle list is using UVs.
    int                             mode;       // Drawing mode (Default: GL_TRIANGLES).
    unsigned int                    vbo;
};

class Obj;

// Holds the unique vertex data index
struct ObjVertexData{ int vIndex, uvIndex; };

class ObjMesh{
    friend class Obj;
public:
    ObjMesh();
    ~ObjMesh();
    
    unsigned int draw();
    void build();
    void setAttributes();
    
    RenderObjectType renderObjectType();
    
private:
    // Fields
    std::string                     name;           // Mesh name.
    std::string                     group;
    bool                            visible;        // If true - it's visible.
    std::vector<ObjVertexData>      vertexData;     // All vertex data (vertex index & uv index)
    std::vector<sp<ObjTriangleList>>tLists;         // Triangle lists...
    sp<ObjMaterial>                 currentMaterial = nullptr;

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
    
    wp<Obj>                 weakData;
    
    // Helpers
    void    addVertexData(sp<ObjTriangleList> otl, int vIndex, int uvIndex);
    void    buildVBO();
    void    initMaterial();
    void    updateBounds();
    void    updateMin(v3d &min, v3d &vertex);
    void    updateMax(v3d &max, v3d &vertex);
    
    void    clear();
};