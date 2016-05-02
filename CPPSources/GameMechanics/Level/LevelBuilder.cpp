#include "LevelBuilder.hpp"
#include "Scene.hpp"


void Levelbuilder::buildLevel(){
    addNewBlock(v3d(0, 0, 0));
    
    for(unsigned int i = 0; i < blockCount; ++i){
        addNewBlock(calculateNewPoss(mLastBlockPoss));
    }
}


void Levelbuilder::addNewBlock(v3d blockPos){
    if(mesh){
        mLastBlockPoss = blockPos;
        
        up<GameObject> go = std::unique_ptr<GameObject>(new GameObject("block"));
        
        go = std::unique_ptr<GameObject>(new GameObject("bblock_Cube"));
        up<MeshRendererComponent> mrc = up<MeshRendererComponent>(new MeshRendererComponent(go.get(), mesh));
        go->addComponent(ComponentEnum::MESH_RENDERER, std::move(mrc));
        
        up<RigidBodyComponent> rbc_1 = up<RigidBodyComponent>(new RigidBodyComponent(go.get(), 0.0f));
        
        
        btTransform t = rbc_1->mBody->getWorldTransform();
        t.setOrigin(btVector3(blockPos.x, blockPos.y, blockPos.z));
//        t.setPosition(btVector3(blockPos.x, blockPos.y, blockPos.z));
        rbc_1->mBody->setWorldTransform(t);
        
        go->addComponent(ComponentEnum::RIGID_BODY, std::move(rbc_1));
        
        go->mTransform = v3d(blockPos);
        
        Scene::instance()->addObjOnScene(std::move(go));
    }
}

v3d Levelbuilder::calculateNewPoss(v3d lastPos){
    int step = 2;
    int x = rand()%2 > 0 ? step : 0;
    int y = x > 0 ? 0 : step;
    return (lastPos + v3d(x, y, 0));
}