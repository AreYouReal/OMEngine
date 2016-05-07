#include "LevelBuilder.hpp"
#include "Scene.hpp"


void Levelbuilder::buildLevel(std::queue<float> &actions){
    addNewBlock(v3d(0, 0, 0), true);
    srand(time(0));
    bool addArrow = false;
    for(unsigned int i = 0; i < blockCount; ++i){
        v3d newPos = calculateNewPoss(mLastBlockPoss);
        v3d dir = newPos - mLastBlockPoss;
        if(mLastDir != dir){
            addArrow = true;
            mLastDir = dir;
            if(dir.x > 0.1){
                actions.push(0);
            }else{
                if(dir.y < -0.1){
                    actions.push(PI * 3 * 0.5f);
                }else{
                    if(dir.y > 0.1){
                        actions.push(PI * 0.5f);
                    }else{
                        actions.push(PI);
                    }
                }
            }
        }else{
            addArrow = false;
        }
        addNewBlock(newPos, addArrow);
        mLastBlockPoss = newPos;
    }
}


void Levelbuilder::addNewBlock(v3d blockPos, bool addArrow){
    if(mesh){
        up<GameObject> go = std::unique_ptr<GameObject>(new GameObject("bblock_Cube"));
        up<MeshRendererComponent> mrc = up<MeshRendererComponent>(new MeshRendererComponent(go.get(), mesh));
        go->addComponent(ComponentEnum::MESH_RENDERER, std::move(mrc));
        
        up<RigidBodyComponent> rbc_1 = up<RigidBodyComponent>(new RigidBodyComponent(go.get(), 0.0f));
        
        btTransform t = rbc_1->mBody->getWorldTransform();
        t.setOrigin(btVector3(blockPos.x, blockPos.y, blockPos.z));
        rbc_1->mBody->setWorldTransform(t);
        
        go->addComponent(ComponentEnum::RIGID_BODY, std::move(rbc_1));

        up<DebugDrawComponent> ddc = up<DebugDrawComponent>(new DebugDrawComponent(go.get()));
        go->addComponent(ComponentEnum::DEBUG_DRAW, std::move(ddc));
        go->mTransform = v3d(blockPos);
        if(addArrow && prevObj != nullptr) addArrowToBlock(prevObj);
        
        prevObj = go.get();
        Scene::instance()->addObjOnScene(std::move(go));
    }
}

void Levelbuilder::addArrowToBlock(GameObject *parent){
    if(arrow){
        sp<GameObject> go = std::unique_ptr<GameObject>(new GameObject("ArrowObj_Plane"));
        up<MeshRendererComponent> mrc = up<MeshRendererComponent>(new MeshRendererComponent(go.get(), arrow));
        go->addComponent(ComponentEnum::MESH_RENDERER, std::move(mrc));
        go->mTransform = v3d(0, 0, 2);
        parent->addChild(go);
    }
}

v3d Levelbuilder::calculateNewPoss(v3d lastPos){
    int x, y;
    int step = 2;
    if((rand() % 2) > 0){
        x = 0; y = step;
        if((rand() % 2) > 0){
            y = -y;
        }
    }else{
        x = step; y = 0;
        if((rand() % 2) > 0){
            x = -x;
        }
    }
    return (lastPos + v3d(x, y, 0));
}