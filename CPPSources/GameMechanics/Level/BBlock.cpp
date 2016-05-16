#include "BBlock.hpp"

BBlock::BBlock(GameObject * const gameObject, LevelBuilder * const lb) : IComponent(gameObject), mLB(lb){}

BBlock::BBlock(GameObject * const gameObject) : IComponent(gameObject){}



BBlock::~BBlock(){
    onDestroy();
}

void BBlock::reinit(v3d newPosition){
    RigidBodyComponent *rbc = static_cast<RigidBodyComponent*>(go->getComponent(ComponentEnum::RIGID_BODY));
    if(rbc){
        rbc->mBody->translate(btVector3(newPosition.x, newPosition.y, newPosition.z));
    }
}

void BBlock::show(){

}

void BBlock::hide(){
    if(mLB)
        mLB->onHideBlock(go);
}