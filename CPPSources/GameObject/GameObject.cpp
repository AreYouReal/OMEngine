#include "GameObject.hpp"


GameObject::GameObject(){

}

GameObject::~GameObject(){

}


void GameObject::addObjMesh(ObjMesh* objToAdd){
    objMesh = objToAdd;
}

void GameObject::draw(){
    objMesh->draw();
}

