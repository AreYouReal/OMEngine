#pragma once

// SREngine
#include "OMUtils.h"
#include "ObjMaterial.h"
#include "Obj.h"
#include "MeshOutlines.h"
#include "MeshGLInfo.h"

enum class RenderObjectType{ SOLID, ALPHA_TESTED, TRANSPARENT };

// Hold all the vertex and UV indices for the particular triangle.
struct ObjTriangleIndex{ int vertexIndex[3], uvIndex[3]; };

// Represent the one triangle list onside ObjMesh
struct ObjTriangleList{
    ObjTriangleList(){}
    ~ObjTriangleList(){}
    std::vector<ObjTriangleIndex>   tIndices;
    std::vector<unsigned short>     indices;
    sp<ObjMaterial>                 material;  // Pointer to the material to use when draw this list.
    bool                            useUVs = false;     // True if triangle list is using UVs.
    int                             mode = GL_TRIANGLES;       // Drawing mode (Default: GL_TRIANGLES).
    unsigned int                    vbo;
};

class Obj;

// Holds the unique vertex data index
struct ObjVertexData{ int vIndex, uvIndex; };

class ObjMesh{
    friend class Obj;
public:
// Functions
    ObjMesh();
    ~ObjMesh();
    
    unsigned int    draw();
    void            build();
    void            setAttributes();
    
    RenderObjectType renderObjectType();
    std::string     getName(){ return name; }
// Fields
    MeshOutlines                    outlines;
    
    bool        shadowDraw = false;
    
//private:
// Helpers
    void    addVertexData(sp<ObjTriangleList> otl, int vIndex, int uvIndex);
    void    buildVBO();
    void    initMaterial();
    void    updateBounds();
    void    updateMin(v3d &min, v3d &vertex);
    void    updateMax(v3d &max, v3d &vertex);
    void    clear();
    
// Fields
    std::string                     name;           // Mesh name.
    std::string                     group;
    bool                            visible;        // If true - it's visible.
    std::vector<ObjVertexData>      vertexData;     // All vertex data (vertex index & uv index)
    std::vector<sp<ObjTriangleList>>tLists;         // Triangle lists...
    sp<ObjMaterial>                 currentMaterial = nullptr;
    sp<ObjMaterial>                 shadowMaterial   = nullptr;
    wp<Obj>                         weakData;
    MeshGLInfo                      glInfo;
};