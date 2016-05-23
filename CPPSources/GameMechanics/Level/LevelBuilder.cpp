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
    
    addNewBlock(mLastBlockPoss, 0.0f);
    
    for(unsigned int i = 0; i < blockCount; ++i){
        v3d newPos = calculateNewPoss(mLastBlockPoss);
        float rotation = getRotationAngle(newPos, mLastBlockPoss);
        addNewBlock(newPos, rotation);
        logMessage("New rotation: %f\n", rotation);
        mLastBlockPoss = newPos;
    }
    
}

ArrowAction* LevelBuilder::popAction(){
    if(!actions.empty()){
        ArrowAction *action = actions.front();
        actions.pop();
        action->hide();
        for(int i = 0; i < activeArrows.size(); ++i){
            if(activeArrows[i] == action->go){
                inactiveArrows.push_back(activeArrows[i]);
                activeArrows.erase(activeArrows.begin() + i);
                break;
            }
        }
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
        go->setPosition(blockPos);
        if(rotation >= 0 && prevObj != nullptr) addArrowToBlock(prevObj, rotation);
        
        prevObj = go.get();
        addBlockComponent(go.get());
        activeblocks.push_back(go.get());
        
        logMessage("Block POS: [%f, %f, %f]\n", go->getPosition().x, go->getPosition().y, go->getPosition().z );
        
        Scene::instance()->addObjOnScene(std::move(go));
    }
}

void LevelBuilder::addArrowToBlock(GameObject *parent, float rotation){
    if(mArrow){
        up<GameObject> go = std::unique_ptr<GameObject>(new GameObject("ArrowObj_Plane"));
        up<MeshRendererComponent> mrc = up<MeshRendererComponent>(new MeshRendererComponent(go.get(), mArrow));
        go->addComponent(ComponentEnum::MESH_RENDERER, std::move(mrc));
        up<ArrowAction> aa = std::unique_ptr<ArrowAction>(new ArrowAction(go.get(), q4d(rotation, v3d(0, 1, 0))));
        actions.push(aa.get());
        go->mTransform.rotate(aa->mRotation);
        go->mTransform.mPosition = parent->getPosition()+ v3d(0, 2, 0);
        go->mTransform.refreshTransformMatrix();
        go->addComponent(ComponentEnum::ACTION_ARROW, std::move(aa));
        activeArrows.push_back(go.get());
        Scene::instance()->addObjOnScene(std::move(go));
    }
}

v3d LevelBuilder::calculateNewPoss(v3d lastPos){
    int x, z;
    int step = 2;
    if((rand() % 2) > 0){
        x = 0; z = step;
//        if((rand() % 2) > 0){
//            y = -y;
//        }
    }else{
        x = step; z = 0;
//        if((rand() % 2) > 0){
//            x = -x;
//        }
    }
    v3d newPos = lastPos + v3d(x, 0, z);
    return newPos;
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
            rotation = 90.0f;
        }else{
            if(dir.z < -0.1){
                rotation = 180.0f;
            }else{
                if(dir.z > 0.1){
                    rotation = 0.0f;
                }else{
                    rotation = 270.0f;
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


        
        for(int i = 0; i < inactiveBlocks.size(); ++i){
            if(inactiveBlocks[i] == go){
                inactiveBlocks.erase(inactiveBlocks.begin() + i);
                activeblocks.push_back(go);
                break;
            }
        }
        float rotation = getRotationAngle(newPos, mLastBlockPoss);
        logMessage("New rotation: %f\n", rotation);
        if(rotation > -1){
            if(inactiveArrows.size() > 0){
                GameObject *arrowGO = inactiveArrows[0];
                inactiveArrows.erase(inactiveArrows.begin());
                activeArrows.push_back(arrowGO);
                ArrowAction *aa = static_cast<ArrowAction*>( arrowGO->getComponent(ComponentEnum::ACTION_ARROW) );
                if(aa){
                    aa->show(mLastBlockPoss + v3d(0, 2, 0), rotation);
                    logMessage("Actions push! \n");
                    actions.push(aa);
                }
            }
        }
        mLastBlockPoss = newPos;
    }
}


void LevelBuilder::refresh(){
    for(auto const &go : activeblocks){
        Scene::instance()->removeObjectFromTheScene(go);
    }
    
    for(auto const &go : activeArrows){
        Scene::instance()->removeObjectFromTheScene(go);
    }
    
    for(auto const &go : inactiveArrows){
        Scene::instance()->removeObjectFromTheScene(go);
    }
    
    for(auto const &go : inactiveBlocks){
        Scene::instance()->removeObjectFromTheScene(go);
    }
    
    
    activeblocks.clear();
    inactiveBlocks.clear();
    activeArrows.clear();
    inactiveArrows.clear();
    prevObj = nullptr;
    
    actions = std::queue<ArrowAction*>{};
    
    mLastBlockPoss = v3d(0, 0, 0);
    mLastDir = v3d(0, 0, 0);
    buildLevel();
}
