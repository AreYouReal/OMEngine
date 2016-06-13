#include "AssetManager.hpp"

const std::string actionObjName{"candies.obj"};
const std::string bblockObjName{"bblock.obj"};

const string actionArrowMeshName{"candy_1"};
const string bblockMeshName{"bblock"};

const string playBtnObjName("play_btn.obj");
const string playBtnMeshName("play_btn");


void AssetManager::init(){
    loadPlayBtnObj();
    loadBlockObj();
    loadArrowObj();
}

std::vector<sp<ObjMesh>> AssetManager::getAllMeshesFromObj(string objFileName){
    if(mObjRess.find(objFileName) != mObjRess.end()){
        return mObjRess[objFileName]->getAllMeshes();
    }
    return std::vector<sp<ObjMesh>>();
}

sp<ObjMesh> AssetManager::getMeshFromObj(string objFileName, string meshName){
    if(mObjRess.find(objFileName) != mObjRess.end()){
        return mObjRess[objFileName]->getMesh(meshName);
    }
    return nullptr;
}

void AssetManager::loadPlayBtnObj(){
    sp<Obj> playBtnObj = Obj::load(playBtnObjName.c_str());
    playBtnObj->build();
    playBtnObj->clear();
    
    mObjRess.insert(std::pair<string, sp<Obj>>(playBtnObjName, playBtnObj ));
}

void AssetManager::loadBlockObj(){
    sp<Obj> bblockObj = Obj::load(bblockObjName.c_str());
    bblockObj->build();
    bblockObj->clear();
    
    mObjRess.insert(std::pair<string, sp<Obj>>(bblockObjName, bblockObj));
}

void AssetManager::loadArrowObj(){
    sp<Obj> arrowObj = Obj::load(actionObjName.c_str());
    arrowObj->build();
    arrowObj->clear();
    
    mObjRess.insert(std::pair<string, sp<Obj>>(actionObjName, arrowObj));
}