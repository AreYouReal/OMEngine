#include "LevelBuilder.hpp"
#include "Scene.hpp"
#include "BBlock.hpp"

#pragma mark IComponent Interface Related
LevelBuilder::LevelBuilder(GameObject * const gameObject) : IComponent(gameObject){}

LevelBuilder::~LevelBuilder(){
    onDestroy();
}

void LevelBuilder::InitWithMeshes(sp<ObjMesh> block, sp<ObjMesh> arrow){
    mBlock = block;
    mArrow = arrow;
}


#pragma mark Public
void LevelBuilder::buildLevel(){
    srand(time(0));
    
    addNewBlock(v3d(0, 0, 0), 0.0f);
    
    for(unsigned int i = 0; i < blockCount; ++i){
        v3d newPos = calculateNewPoss(mLastBlockPoss);
        float rotation = getRotationAngle(newPos, mLastBlockPoss);
        
        addNewBlock(newPos, rotation);

        mLastBlockPoss = newPos;
    }
    
}

ArrowAction* LevelBuilder::popAction(){
    if(!actions.empty()){
        ArrowAction *action = actions.front();
        actions.pop();
        action->hide();
        return action;
    }
    return nullptr;
}

void LevelBuilder::onHideBlock(GameObject *blockOBj){
    for(int i = 0; i < activeblocks.size(); ++i){
        if(activeblocks[i] == blockOBj){
            activeblocks.erase(activeblocks.begin() + i);
            (static_cast<MeshRendererComponent*>(blockOBj->getComponent(ComponentEnum::MESH_RENDERER)))->visible = false;
            inactiveBlocks.push_back(blockOBj);
            break;
        }
    }
}

void LevelBuilder::update(){
    if(!inactiveBlocks.empty()){
        activateBlock(inactiveBlocks[0]);
//        inactiveBlocks.erase(inactiveBlocks.begin());
        logMessage("inactiveBLock size: %d\n", inactiveBlocks.size());
    }
}

#pragma mark Private Helpers
void LevelBuilder::addNewBlock(v3d blockPos, float rotation){
    if(mBlock){
        up<GameObject> go = std::unique_ptr<GameObject>(new GameObject("bblock_Cube"));
        up<MeshRendererComponent> mrc = up<MeshRendererComponent>(new MeshRendererComponent(go.get(), mBlock));
        go->addComponent(ComponentEnum::MESH_RENDERER, std::move(mrc));
        
        up<RigidBodyComponent> rbc_1 = up<RigidBodyComponent>(new RigidBodyComponent(go.get(), 0.0f));
        
        btTransform t = rbc_1->mBody->getWorldTransform();
        t.setOrigin(btVector3(blockPos.x, blockPos.y, blockPos.z));
        rbc_1->mBody->setWorldTransform(t);
        
        go->addComponent(ComponentEnum::RIGID_BODY, std::move(rbc_1));

        up<DebugDrawComponent> ddc = up<DebugDrawComponent>(new DebugDrawComponent(go.get()));
        go->addComponent(ComponentEnum::DEBUG_DRAW, std::move(ddc));
        go->mTransform = v3d(blockPos);
        if(rotation >= 0 && prevObj != nullptr) addArrowToBlock(prevObj, rotation);
        
        prevObj = go.get();
        addBlockComponent(go.get());
        activeblocks.push_back(go.get());
        
        Scene::instance()->addObjOnScene(std::move(go));
    }
}

void LevelBuilder::addArrowToBlock(GameObject *parent, float rotation){
    if(mArrow){
        up<GameObject> go = std::unique_ptr<GameObject>(new GameObject("ArrowObj_Plane"));
        up<MeshRendererComponent> mrc = up<MeshRendererComponent>(new MeshRendererComponent(go.get(), mArrow));
        go->addComponent(ComponentEnum::MESH_RENDERER, std::move(mrc));
        up<ArrowAction> aa = std::unique_ptr<ArrowAction>(new ArrowAction(go.get(), q4d(rotation, v3d(0, 0, 1))));
        actions.push(aa.get());
        go->mTransform.rotate(aa->rotation);
        go->mTransform.mPosition = parent->mTransform.mPosition + v3d(0, 0, 2);
        go->mTransform.refreshTransformMatrix();
        go->addComponent(ComponentEnum::ACTION_ARROW, std::move(aa));
        Scene::instance()->addObjOnScene(std::move(go));
    }
}

v3d LevelBuilder::calculateNewPoss(v3d lastPos){
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

void LevelBuilder::addBlockComponent(GameObject *go){
    up<BBlock> block = std::unique_ptr<BBlock>(new BBlock(go, this));
    go->addComponent(ComponentEnum::BBLOCK, std::move(block));
}


float LevelBuilder::getRotationAngle(v3d newPos, v3d lastDir){
    v3d dir = newPos - mLastBlockPoss;
    float rotation = -1.0f;
    if(mLastDir != dir){
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
        return rotation;
    }
    return -1.0f;
}

void LevelBuilder::activateBlock(GameObject *go){
    BBlock *blockComp = static_cast<BBlock*>(go->getComponent(ComponentEnum::BBLOCK));
    if(blockComp){
        v3d newPos = calculateNewPoss(mLastBlockPoss);
        blockComp->show(newPos);
        MeshRendererComponent *mmc = static_cast<MeshRendererComponent*>(go->getComponent(ComponentEnum::MESH_RENDERER));
        if(mmc){
            mmc->visible = true;
        }
        mLastBlockPoss = newPos;
        for(int i = 0; i < inactiveBlocks.size(); ++i){
            if(inactiveBlocks[i] == go){
                inactiveBlocks.erase(inactiveBlocks.begin() + i);
                activeblocks.push_back(go);
                break;
            }
        }
    
    }
}