
#include "Materials.hpp"

#pragma mark Singleton related
std::shared_ptr<Materials> Materials::mInstance = NULL;

std::shared_ptr<Materials> Materials::instance(){
    if(!mInstance){
        mInstance = std::shared_ptr<Materials>(new Materials());
    }
    return mInstance;
}



Materials::Materials(){ }


bool Materials::loadMaterial(const std::string &name){

    return true;
}

OBJMATERIAL Materials::getMaterial(const std::string &name){
    if(materials.find(name) != materials.end()) return materials[name];
    return nullptr;
}

