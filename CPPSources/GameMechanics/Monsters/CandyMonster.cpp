#include "CandyMonster.hpp"

CandyMonster::CandyMonster(GameObject * const gameObject) : IComponent(gameObject){};

CandyMonster::~CandyMonster(){}


up<GameObject> CandyMonster::create(CandyMonster::CandyType type){
    string monsterName = "minimon_" + std::to_string( (int) type);
    
    up<GameObject> monster = std::unique_ptr<GameObject>(new GameObject(monsterName));
    std::vector<string>  monsterActions;
    monsterActions.clear();

    monsterActions.push_back(monsterName + "_idle.md5anim");
    monsterActions.push_back(monsterName + "_run.md5anim");
    monsterActions.push_back(monsterName + "_jump.md5anim");
    
    up<AnimMeshComponent> mamc = up<AnimMeshComponent>(new AnimMeshComponent(monster.get(), monsterName + ".md5mesh", monsterName + ".mtl", monsterActions));
    mamc->mComponentType = ComponentEnum::ANIM_MESH;
    monster->addComponent(std::move(mamc));
    
    q4d sR(180, v3d(0, 1, 0));
    monster->mTransform.rotate(sR);
    monster->mTransform.translate(0, -.85f, 0);
    monster->mTransform.mScale = v3d(3, 3, 3);
    monster->mTransform.refreshTransformMatrix();
    
//    up<RigidBodyComponent> rbc_1 = up<RigidBodyComponent>(new RigidBodyComponent(monster.get(), 5.0f));
//    rbc_1->mBody->setGravity(btVector3(0, 0, 0));
//    rbc_1->mComponentType = ComponentEnum::RIGID_BODY;
//    monster->addComponent(std::move(rbc_1));
    
    return std::move(monster);
}
