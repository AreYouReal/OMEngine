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
    
    addNewBlock(mLastBlockPoss, LAction(LAction::NONE));
    
    for(unsigned int i = 0; i < blockCount; ++i){
        v3d newPos = calculateNewPoss(mLastBlockPoss);
        LAction action = getAction(newPos, mLastBlockPoss);
        addNewBlock(newPos, action);
    }
    
}

LAction LevelBuilder::popAction(){
    if(!actions.empty()){
        LAction action = actions.front();
        actions.pop();
        return action;
    }
    LAction act(LAction::Type::YAW, q4d(180, v3d(0, 1, 0)));
    return act;
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
    }
}

#pragma mark Private Helpers
void LevelBuilder::addNewBlock(v3d blockPos, LAction action){
    if(mBlock){
        up<GameObject> go = std::unique_ptr<GameObject>(new GameObject("bblock_Cube"));
        up<MeshRendererComponent> mrc = up<MeshRendererComponent>(new MeshRendererComponent(go.get(), mBlock));
        go->addComponent(ComponentEnum::MESH_RENDERER, std::move(mrc));
        
        up<RigidBodyComponent> rbc_1 = up<RigidBodyComponent>(new RigidBodyComponent(go.get(), 0.0f));
        
        btTransform t = rbc_1->mBody->getWorldTransform();
        t.setOrigin(btVector3(blockPos.x, blockPos.y, blockPos.z));
        rbc_1->mBody->setWorldTransform(t);
        
        go->addComponent(ComponentEnum::RIGID_BODY, std::move(rbc_1));

//        up<DebugDrawComponent> ddc = up<DebugDrawComponent>(new DebugDrawComponent(go.get()));
//        go->addComponent(ComponentEnum::DEBUG_DRAW, std::move(ddc));
        go->setPosition(blockPos);
        if(action.mType != LAction::Type::NONE && prevObj != nullptr) addCandyToBlock(action);
        
        prevObj = go.get();
        addBlockComponent(go.get());
        activeblocks.push_back(go.get());
        
        Scene::instance()->addObjOnScene(std::move(go));
        mLastBlockPoss = blockPos;
    }
}

void LevelBuilder::addCandyToBlock(LAction action){
    if(mArrow){
        up<GameObject> go = std::unique_ptr<GameObject>(new GameObject("ArrowObj_Plane"));
        up<MeshRendererComponent> mrc = up<MeshRendererComponent>(new MeshRendererComponent(go.get(), mArrow));
        go->addComponent(ComponentEnum::MESH_RENDERER, std::move(mrc));
        up<LevelRelated::Candy> aa = std::unique_ptr<LevelRelated::Candy>(new LevelRelated::Candy(go.get()));
        logMessage("Add Action %d\n", action.mType);
        actions.push(action);
        go->mTransform.mPosition = prevObj->getPosition()+ v3d(0, 2, 0);
        go->mTransform.refreshTransformMatrix();
        go->addComponent(ComponentEnum::CANDY, std::move(aa));
        activeCandies.push_back(go.get());
        Scene::instance()->addObjOnScene(std::move(go));
    }
}

v3d LevelBuilder::calculateNewPoss(v3d lastPos){
    int x, z;
    int step = 2;
    int randValue = rand() % 10;
    if(randValue > 3){
        x = 0; z = step;
//        if((rand() % 2) > 0){
//            y = -y;
//        }
    }else if(randValue > 7){
        x = step; z = 0;
    }else {
        x = 2 * step;
//        if((rand() % 2) > 0){
//            x = -x;
//        }
        z = 0;
    }
    v3d newPos = lastPos + v3d(x, 0, z);
    return newPos;
}

void LevelBuilder::addBlockComponent(GameObject *go){
    up<BBlock> block = std::unique_ptr<BBlock>(new BBlock(go, this));
    go->addComponent(ComponentEnum::BBLOCK, std::move(block));
}


LAction LevelBuilder::getAction(v3d newPos, v3d lastDir){
    v3d dir = newPos - mLastBlockPoss;
    float rotation = -1.0f;
    LAction action(LAction::NONE);
    
    float length = v3d::length(mLastBlockPoss - newPos) ;
    
    
    logMessage("PREV: ");
    v3d::print(mLastBlockPoss);
    logMessage("CURRENT: ");
    v3d::print(newPos);
    logMessage("Length: %f\n", length);

    
    if(length > 3.0f){
        action.mType = LAction::Type::JUMP;
        action.mMagnitude = 7500.0f;
    }
    
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
        
        if(action.mType == LAction::Type::JUMP){
            action.mType = LAction::Type::YAW_JUMP;
        }else{
            action.mType = LAction::Type::YAW;
        }
        
        action.mRotation = q4d(rotation, v3d(0, 1, 0));
    }
    return action;
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
        LAction action = getAction(newPos, mLastBlockPoss);

        if(action.mType != LAction::Type::NONE){
            if(inactiveCandies.size() > 0){
                GameObject *arrowGO = inactiveCandies[0];
                inactiveCandies.erase(inactiveCandies.begin());
                activeCandies.push_back(arrowGO);
                LevelRelated::Candy *aa = static_cast<LevelRelated::Candy*>( arrowGO->getComponent(ComponentEnum::CANDY) );
                if(aa){
                    aa->show(mLastBlockPoss + v3d(0, 2, 0));
                }
            }
            actions.push(action);
        }

    }
}


void LevelBuilder::refresh(){
    clearLevel();
    buildLevel();
}

void LevelBuilder::clearLevel(){
    for(auto const &go : activeblocks){
        Scene::instance()->removeObjectFromTheScene(go);
    }
    
    for(auto const &go : activeCandies){
        Scene::instance()->removeObjectFromTheScene(go);
    }
    
    for(auto const &go : inactiveCandies){
        Scene::instance()->removeObjectFromTheScene(go);
    }
    
    for(auto const &go : inactiveBlocks){
        Scene::instance()->removeObjectFromTheScene(go);
    }
    
    
    activeblocks.clear();
    inactiveBlocks.clear();
    activeCandies.clear();
    inactiveCandies.clear();
    prevObj = nullptr;
    
    actions = std::queue<LAction>{};
    
    mLastBlockPoss = v3d(0, 0, 0);
    mLastDir = v3d(0, 0, 0);
}
