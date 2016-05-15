#include "BBlock.hpp"


BBlock::BBlock(GameObject * const gameObject) : IComponent(gameObject){}

BBlock::~BBlock(){
    onDestroy();
}

void BBlock::reinit(v3d newPosition){

}

void BBlock::show(){

}

void BBlock::hide(){
    
}