#include "PlayButton.hpp"
#include "GameObject.hpp"

#include "AssetManager.hpp"

#include "MeshRendererComponent.hpp"
#include "RigidBodyComponent.hpp"


up<GameObject> PlayButton::create(){
    up<GameObject> play = up<GameObject>(new GameObject("PLAY"));
    up<MeshRendererComponent> mrc = up<MeshRendererComponent>(new MeshRendererComponent(play.get(), AssetManager::instance()->getMeshFromObj("bblock.obj", "bblock")));
    play->addComponent(std::move(mrc));

    up<RigidBodyComponent> rbc = up<RigidBodyComponent>(new RigidBodyComponent(play.get(), 0.0f));
    
    rbc->mBody->setCollisionFlags(rbc->mBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    
    play->addComponent(std::move(rbc));
    
    play->setPosition(v3d(0, 8, 0));
    up<PlayButton> pbc = up<PlayButton>(new PlayButton(play.get()));
    
    pbc->mRigidBodyComp = static_cast<RigidBodyComponent*>( play->getComponent(ComponentEnum::RIGID_BODY) );
    
    play->addComponent(std::move(pbc));

    return play;
}

PlayButton::PlayButton(GameObject * const gameObject) : IComponent(gameObject){
    mComponentType = ComponentEnum::PLAY_BTN;
}


void PlayButton::update(){ }