#include "PlayButton.hpp"
#include "GameObject.hpp"

#include "AssetManager.hpp"

#include "MeshRendererComponent.hpp"
#include "RigidBodyComponent.hpp"


up<GameObject> PlayButton::create(){
    up<GameObject> play = up<GameObject>(new GameObject("PLAY"));
    up<MeshRendererComponent> mrc = up<MeshRendererComponent>(new MeshRendererComponent(play.get(), AssetManager::instance()->getMeshFromObj("play_btn.obj", "play_btn")));
    play->addComponent(std::move(mrc));

    up<RigidBodyComponent> rbc = up<RigidBodyComponent>(new RigidBodyComponent(play.get(), 0.0f));
    
    rbc->mBody->setCollisionFlags(rbc->mBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    
    play->addComponent(std::move(rbc));
    
    
    
    play->setPosition(v3d(0, 8, 2));
    up<PlayButton> pbc = up<PlayButton>(new PlayButton(play.get()));
    
    pbc->mRigidBodyComp = static_cast<RigidBodyComponent*>( play->getComponent(ComponentEnum::RIGID_BODY) );
    
    play->addComponent(std::move(pbc));

    return play;
}

PlayButton::PlayButton(GameObject * const gameObject) : IComponent(gameObject){
    mComponentType = ComponentEnum::PLAY_BTN;
}


void PlayButton::update(){
    static float rotAngle = 0.0f;
    
    rotAngle += 50 * Time::deltaTime;
    if(rotAngle > 360) rotAngle = 0;
    
    btTransform transform = mRigidBodyComp->mBody->getWorldTransform();
    q4d currentRotation(rotAngle, v3d(0, 1, 0));
    q4d rotation = currentRotation;
    btQuaternion cr(rotation.x, rotation.y, rotation.z, rotation.w);
    transform.setRotation(cr);
    mRigidBodyComp->mBody->setCenterOfMassTransform(transform);
}