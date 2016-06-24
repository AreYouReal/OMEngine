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
    loadWorldLevelInfo();
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

void AssetManager::loadWorldLevelInfo(){
    std::unique_ptr<FileContent> worldContent = readTextFile("worldA.cm");
    string content = std::string( reinterpret_cast<const char*>(worldContent->content) );
    world = LevelInfo::parseWorld(content);
}

sp<md5::MD5> AssetManager::loadMD5Mesh(string name, std::vector<string> actionsToLoad, string materialname){
    Materials::instance()->loadMaterial(materialname);
    string fileName = name;
#ifdef ANDROID
    fileName = "animations/" + fileName;
#endif
    sp<md5::MD5> md5 = md5::MD5::loadMesh(fileName);
    std::size_t pos = name.find(".md5mesh");
    string meshName = name.substr(0, pos);
    if(md5){
        md5->optimize(128);
        md5->build();
        md5->freeMeshData();
        mMD5Ress.insert(std::pair<string, sp<md5::MD5>>(meshName, md5));
    }
    
    for(auto const &name : actionsToLoad){
        string fileName = name;
#ifdef ANDROID
        fileName = "animations/" + fileName;
#endif
        std::size_t pos = name.find(".md5anim");
        string actionName = name.substr(0, pos);
        md5->loadAction(actionName, fileName);
    }
    
    return md5;
}

sp<LevelInfo> AssetManager::getLevelInfo(int num){
    if(num < 0 || num >= world.size()){
        return nullptr;
    }
    return world[num];
}