#include "GameObject.hpp"


GameObject::GameObject(){

}

GameObject::~GameObject(){

}


void GameObject::addObjMesh(OBJMESH objToAdd){
    objMesh = objToAdd;
}

void GameObject::draw(){
    objMesh->draw();
}

