#pragma once

#include "Singleton.hpp"
#include "Shortcuts.h"
#include "GameObject.hpp"
#include "OMGame.h"
#include "md5.hpp"
#include "LevelInfo.hpp"

class AssetManager : public Singleton<AssetManager>{

public:
    void init();
    
    void loadPlayBtnObj();
    void loadBlockObj();
    void loadArrowObj();
    void loadWorldLevelInfo();
    
    sp<ObjMesh> getMeshFromObj(string objFileName, string meshName);
    
    std::vector<sp<ObjMesh>> getAllMeshesFromObj(string objFileName);
    
    sp<md5::MD5> loadMD5Mesh(string filename, std::vector<string> actionsToLoad, string materialname);
    sp<md5::MD5> getMD5Mesh(const string name);
    
    
    sp<LevelInfo> getLevelInfo(int num);
    
private:
    
    std::map<string, sp<Obj>> mObjRess;
    std::map<string, sp<md5::MD5>> mMD5Ress;
    vec<sp<LevelInfo>>  world;
    
    
};