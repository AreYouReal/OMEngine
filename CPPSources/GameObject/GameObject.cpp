#include "GameObject.hpp"


GameObject::GameObject(){

}

GameObject::~GameObject(){

}


void GameObject::addObjMesh(sp<ObjMesh> objToAdd){
    objMesh = objToAdd;
}

