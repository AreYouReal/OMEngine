#pragma once

#include "Singleton.hpp"
#include "Shortcuts.h"
#include "GameObject.hpp"
#include "OMGame.h"

class AssetManager : public Singleton<AssetManager>{

public:
    
    void loadPlayBtnObj();
    void loadBlockObj();
    void loadArrowObj();
    
    sp<ObjMesh> getMeshFromObj(string objFileName, string meshName);
    
    std::vector<sp<ObjMesh>> getAllMeshesFromObj(string objFileName);
    
private:

    std::map<string, sp<Obj>> mObjRess;
    
};