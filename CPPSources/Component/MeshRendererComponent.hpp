#pragma once

#include "Shortcuts.h"
#include "IComponent.hpp"
#include "ObjMesh.h"
#include "GameObject.hpp"

class MeshRendererComponent : public IComponent{
public:
    MeshRendererComponent( GameObject * const gameObject);
    MeshRendererComponent( GameObject * const gameObject, const sp<ObjMesh> mesh);
    MeshRendererComponent( GameObject * const gameObject, const std::vector<sp<ObjMesh>> &meshes);
    
    virtual ~MeshRendererComponent();
    
    virtual bool init()     override;
    virtual void update()   override;
    virtual void onDestroy()  override;

    void addMesh(sp<ObjMesh> mesh);
    
private:
    std::vector<sp<ObjMesh>> mMeshes;
};