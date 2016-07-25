#include "PlayButton.hpp"
#include "GameObject.hpp"

#include "AssetManager.hpp"

#include "MeshRendererComponent.hpp"
#include "RigidBodyComponent.hpp"


up<GameObject> PlayButton::create(){
    up<GameObject> play = up<GameObject>(new GameObject("PLAY"));
    up<MeshRendererComponent> mrc = up<MeshRendererComponent>(new MeshRendererComponent(play.get(), AssetManager::instance()->getMeshFromObj("bblock.obj", "bblock")));
    mrc->castShadows = true;
    play->addComponent(std::move(mrc));

    up<RigidBodyComponent> rbc = up<RigidBodyComponent>(new RigidBodyComponent(play.get(), 0.0f));
    
    rbc->mBody->setCollisionFlags(rbc->mBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    
    play->addComponent(std::move(rbc));
    

    up<PlayButton> pbc = up<PlayButton>(new PlayButton(play.get()));
    
    play->addComponent(std::move(pbc));

    play->setPosition(v3d(0, 8, 0));
    
    return play;
}

PlayButton::PlayButton(GameObject * const gameObject) : IComponent(gameObject){
    mComponentType = ComponentEnum::PLAY_BTN;
}


void PlayButton::update(){ }