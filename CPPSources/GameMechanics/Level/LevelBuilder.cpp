#include "LevelBuilder.hpp"
#include "Scene.hpp"
#include "BBlock.hpp"

#pragma mark IComponent Interface Related
LevelBuilder::LevelBuilder(GameObject * const gameObject) : IComponent(gameObject){
    mComponentType = ComponentEnum::LEVEL_BUILDER;
}

LevelBuilder::~LevelBuilder(){
    onDestroy();
}

void LevelBuilder::InitWithMeshes(sp<ObjMesh> block, std::vector<sp<ObjMesh>> candies){
    mBlock = block;
    mCandies = candies;
}

up<GameObject> LevelBuilder::create(){
    up<GameObject> go = std::unique_ptr<GameObject>(new GameObject("LevelBuilder"));
    up<LevelBuilder> lb = std::unique_ptr<LevelBuilder>(new LevelBuilder(go.get()));
    lb->InitWithMeshes(AssetManager::instance()->getMeshFromObj("bblock.obj", "bblock"), AssetManager::instance()->getAllMeshesFromObj("candies.obj"));
    go->addComponent(std::move(lb));
    return go;
}

#pragma mark Public

void LevelBuilder::buildLevel(){
    srand(time(0));
    
    fillLevelPositions();
    
    for(unsigned int i = 0; i < blockCount; ++i){
        v3d newPos = getNewPos();
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

void LevelBuilder::onHideCandy(GameObject *candyOBj){
    for(int i = 0; i < activeCandies.size(); ++i){
        if(activeCandies[i] == candyOBj){
            activeCandies.erase(activeCandies.begin() + i);
            (static_cast<MeshRendererComponent*>(candyOBj->getComponent(ComponentEnum::MESH_RENDERER)))->visible = false;
            inactiveCandies.push_back(candyOBj);
            break;
        }
    }
}

void LevelBuilder::update(){
    if(!inactiveBlocks.empty()){
        activateBlock(inactiveBlocks[0]);
//        inactiveBlocks.erase(inactiveBlocks.begin());
    }
    
    if(mAimColor.w != 0.0f){
        changeColorRoutine();
    }
}

#pragma mark Private Helpers
void LevelBuilder::addNewBlock(v3d blockPos, LAction action){
    if(mBlock){
        up<GameObject> go = std::unique_ptr<GameObject>(new GameObject("bblock_Cube"));
        up<MeshRendererComponent> mrc = up<MeshRendererComponent>(new MeshRendererComponent(go.get(), mBlock));
        mrc->IComponent::mComponentType = ComponentEnum::MESH_RENDERER;
        go->addComponent(std::move(mrc));
        
        up<RigidBodyComponent> rbc_1 = up<RigidBodyComponent>(new RigidBodyComponent(go.get(), 0.0f));
        
        btTransform t = rbc_1->mBody->getWorldTransform();
        t.setOrigin(btVector3(blockPos.x, blockPos.y, blockPos.z));
        rbc_1->mBody->setWorldTransform(t);
        rbc_1->mComponentType = ComponentEnum::RIGID_BODY;
        go->addComponent(std::move(rbc_1));

        
        if(OMGame::debugFlag){
            up<DebugDrawComponent> ddc = up<DebugDrawComponent>(new DebugDrawComponent(go.get()));
            ddc->mComponentType = ComponentEnum::DEBUG_DRAW;
            go->addComponent(std::move(ddc));
        }

        go->setPosition(blockPos);
        
        if(action.mType != LAction::Type::NONE){
            actions.push(action);

        }
        
        if(prevObj != nullptr){
            addCandyToBlock();
        }
        
        prevObj = go.get();
        addBlockComponent(go.get());
        activeblocks.push_back(go.get());
        
        Scene::instance()->addObjOnScene(std::move(go));
        mLastBlockPoss = blockPos;
    }
}

void LevelBuilder::addCandyToBlock(){
    if(mCandies.size() > 0 && ((rand() % 100) < mCandyChance)){
        up<GameObject> go = std::unique_ptr<GameObject>(new GameObject("Candy"));
        up<MeshRendererComponent> mrc = up<MeshRendererComponent>(new MeshRendererComponent(go.get(), mCandies[rand()%mCandies.size()]));
        mrc->mComponentType = ComponentEnum::MESH_RENDERER;
        go->addComponent(std::move(mrc));
        up<LevelRelated::Candy> aa = std::unique_ptr<LevelRelated::Candy>(new LevelRelated::Candy(go.get(), this));
        aa->mComponentType = ComponentEnum::CANDY;
        go->mTransform.mScale = v3d(0.5f, 0.5f, 0.5f);
        go->mTransform.refreshTransformMatrix();
        go->addComponent(std::move(aa));
        
        up<RigidBodyComponent> rbc = up<RigidBodyComponent>(new RigidBodyComponent(go.get(), 0.0f));
        rbc->mBody->setCollisionFlags(rbc->mBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE );
        rbc->mComponentType = ComponentEnum::RIGID_BODY;
        go->addComponent(std::move(rbc));
        
        go->setPosition(prevObj->getPosition()+ v3d(0, 2, 0));
        
        if(OMGame::debugFlag){
            up<DebugDrawComponent> ddc = up<DebugDrawComponent>(new DebugDrawComponent(go.get()));
            ddc->mComponentType = ComponentEnum::DEBUG_DRAW;
            go->addComponent(std::move(ddc));
        }

        
        activeCandies.push_back(go.get());
        Scene::instance()->addObjOnScene(std::move(go));
    }
}

v3d LevelBuilder::getNewPos(){
    if(mPoss.size() == 0){
        ++currentLevel;
        fillLevelPositions();
    }
    if(mPoss.size() > 0){
        v3d retVal = mPoss.front();
        mPoss.pop();
        return retVal;
    }
    
    int x, z;
    int step = 2;
    int randValue = rand() % 10;
    
    if(randValue < 9){
        if(randValue> 5){
            x = 0; z = step;
        }else{
            x = step; z = 0;
        }
    }else{
        if(randValue % 2 == 0){
            x = 3 * step;         z = 0;
        }else{
            z = 3 * step; x = 0;
        }
    }
    v3d newPos = mLastBlockPoss + v3d(x, 0, z);
    return newPos;
}

void LevelBuilder::addBlockComponent(GameObject *go){
    up<BBlock> block = std::unique_ptr<BBlock>(new BBlock(go, this));
    block->mComponentType = ComponentEnum::BBLOCK;
    go->addComponent(std::move(block));
}


LAction LevelBuilder::getAction(v3d newPos, v3d lastDir){
    v3d dir = newPos - mLastBlockPoss;

    LAction action(LAction::NONE);
    
    float length = v3d::length(mLastBlockPoss - newPos) ;
    
    if(length > 3.0f){
        action.mType = LAction::Type::JUMP;
        action.mMagnitude = 700.0f;
    }
    
    if(mLastDir != dir){
        float rotation = -1.0f;
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
        v3d newPos = getNewPos();
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
        mLastBlockPoss = newPos;
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
    currentLevel = 0;
    mPoss = std::queue<v3d>();
}

void LevelBuilder::fillLevelPositions(){
    mCurrentLevelInfo = AssetManager::instance()->getLevelInfo(currentLevel);
    if(mCurrentLevelInfo){
        Camera::instance()->follow(mCurrentLevelInfo->camPos);
        Camera::instance()->clearColor(mCurrentLevelInfo->bgColor, 3.0f);
        blockMat = Materials::instance()->getMaterial("BBlockMTL");
        mAimColor = mCurrentLevelInfo->bblockColor;
        mChangeColorTime = 3.0f;
        mChangeColorCurrentTime = mChangeColorTime;
        v3d offset = v3d(0, 0, 0);
        if(currentLevel > 0){
            offset.z = -2;
        }
        v3d delta = mCurrentLevelInfo->poss[0] - mLastBlockPoss + offset;
        v3d::print(mLastBlockPoss);
        v3d::print(mCurrentLevelInfo->poss[0]);
        for(auto const &pos : mCurrentLevelInfo->poss){
            mPoss.push(pos - delta);
        }
    }
}

void LevelBuilder::changeColorRoutine(){
    if(mAimColor.w != 0.0f){
        if(mChangeColorCurrentTime <= 0.0f){
            blockMat->diffuse = mAimColor;
            mAimColor = v4d(0, 0, 0, 0);
            mChangeColorTime = -1.0f;
        }else{
            float percent = (mChangeColorTime - mChangeColorCurrentTime) / mChangeColorTime;
            blockMat->diffuse = v4d::lerp(blockMat->diffuse, mAimColor, percent * Time::deltaTime);
        }
        mChangeColorCurrentTime -= Time::deltaTime;
    }
}
