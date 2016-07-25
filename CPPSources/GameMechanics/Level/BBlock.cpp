#include "BBlock.hpp"

BBlock::BBlock(GameObject * const gameObject, LevelBuilder * const lb) : IComponent(gameObject), mLB(lb){
    gameObject->mTransform.mScale = v3d(0.2f, 0.2f, 0.2f);
}

BBlock::BBlock(GameObject * const gameObject) : IComponent(gameObject){}

BBlock::~BBlock(){
    onDestroy();
}

void BBlock::setState(BBlock::BlockState newState){
    mState = newState;
}

void BBlock::show(v3d newPosition){
    RigidBodyComponent *rbc = static_cast<RigidBodyComponent*>(go->getComponent(ComponentEnum::RIGID_BODY));
    if(rbc){
        rbc->mBody->setMassProps(0, btVector3(0, 0, 0));
        btTransform t = rbc->mBody->getWorldTransform();
        t.setOrigin(btVector3(newPosition.x, newPosition.y, newPosition.z));
        rbc->mBody->setCenterOfMassTransform(t);
        rbc->mBody->setFriction(0.0);
    }
}

void BBlock::hide(){
    mState = FADE_OUT;
}

void BBlock::update(){
    switch (mState) {
        case FADE_IN:
//            fadeIn();
            break;
        case NORMAL:
            // do nothing;
//            normal();
            break;
        case FADE_OUT:
            fadeOut();
            break;
    }
}


#pragma mark Helpers
void BBlock::fadeIn(){

}

void BBlock::fadeOut(){
    if(waitTime > 0){
        waitTime -= Time::deltaTime;
    }else{
        if(fadeOutTime > 0){
            fadeOutTime -= Time::deltaTime;
            go->setPosition(go->getPosition() - v3d(0, 10, 0) * Time::deltaTime );
        }else{
            RigidBodyComponent *rbc = static_cast<RigidBodyComponent*>(go->getComponent(ComponentEnum::RIGID_BODY));
            if(rbc){
                rbc->mBody->setMassProps(0, btVector3(0, 0, 0));
            }
            
            mState = NORMAL;
            fadeOutTime = .5f;
            waitTime = 0.5f;
            if(mLB)
                mLB->onHideBlock(go);
        }
    }
}

void BBlock::normal(){

}


