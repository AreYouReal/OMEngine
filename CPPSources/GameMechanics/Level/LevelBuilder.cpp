#include "LevelBuilder.hpp"
#include "Scene.hpp"


void Levelbuilder::buildLevel(std::queue<ArrowAction> &actions){
    srand(time(0));
    bool addArrow = false;
    ArrowAction act;
    addNewBlock(v3d(0, 0, 0), true, &act);
    for(unsigned int i = 0; i < blockCount; ++i){
        v3d newPos = calculateNewPoss(mLastBlockPoss);
        v3d dir = newPos - mLastBlockPoss;
        float rotation = 0.0f;
        if(mLastDir != dir){
            addArrow = true;
            mLastDir = dir;
            if(dir.x > 0.1){
                rotation = 0.0f;
            }else{
                if(dir.y < -0.1){
                    rotation = 270.0f;
                }else{
                    if(dir.y > 0.1){
                        rotation = 90.0f;
                    }else{
                        rotation = 180.0f;
                    }
                }
            }
            act = ArrowAction(nullptr, q4d(rotation, v3d(0, 0, 1)));
        }else{
            addArrow = false;
        }
        addNewBlock(newPos, addArrow, &act);
        if(addArrow) actions.push(act);
        mLastBlockPoss = newPos;
    }
}

void Levelbuilder::addNewBlock(v3d blockPos, bool addArrow, ArrowAction *action){
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
        if(addArrow && prevObj != nullptr) addArrowToBlock(prevObj, action);
        
        prevObj = go.get();
        Scene::instance()->addObjOnScene(std::move(go));
    }
}

void Levelbuilder::addArrowToBlock(GameObject *parent, ArrowAction *action){
    if(arrow){
        up<GameObject> go = std::unique_ptr<GameObject>(new GameObject("ArrowObj_Plane"));
        up<MeshRendererComponent> mrc = up<MeshRendererComponent>(new MeshRendererComponent(go.get(), arrow));
        go->addComponent(ComponentEnum::MESH_RENDERER, std::move(mrc));
        go->mTransform.rotate(action->rotation);
        go->mTransform.mPosition = parent->mTransform.mPosition + v3d(0, 0, 2);
        go->mTransform.refreshTransformMatrix();
        action->arrowObj = go.get();
        Scene::instance()->addObjOnScene(std::move(go));
    }
}

v3d Levelbuilder::calculateNewPoss(v3d lastPos){
    int x, y;
    int step = 2;
    if((rand() % 2) > 0){
        x = 0; y = step;
//        if((rand() % 2) > 0){
//            y = -y;
//        }
    }else{
        x = step; y = 0;
//        if((rand() % 2) > 0){
//            x = -x;
//        }
    }
    return (lastPos + v3d(x, y, 0));
}