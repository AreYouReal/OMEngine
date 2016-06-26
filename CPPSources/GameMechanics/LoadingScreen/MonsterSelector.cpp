#include "MonsterSelector.hpp"
#include "Shortcuts.h"
#include "GameObject.hpp"

void MonsterSelector::update(){
    if(toAngle == currentAngle) return;
    
    int multiplier = currentAngle > toAngle ? -1 : 1;
    
    currentAngle += multiplier * 150.0f * Time::deltaTime;
    if(multiplier > 0){
        if(currentAngle > toAngle){
            currentAngle = toAngle;
        }
    }else{
        if(currentAngle < toAngle){
            currentAngle = toAngle;
        }
    }

    if(currentAngle >= 360){
        currentAngle = 0.0f;
        toAngle = 0.0f;
    }
    go->mTransform.rotate(currentAngle, v3d(0, 1, 0));
}

MonsterSelector* MonsterSelector::add(GameObject *const go){
    up<MonsterSelector> component = up<MonsterSelector>(new MonsterSelector(go));
    component->mComponentType = ComponentEnum::MONSTER_SELECTOR;
    MonsterSelector * comp = component.get();
    go->addComponent(std::move(component));
    return comp;
}

CandyMonster::CandyType MonsterSelector::getCurrentSelectedMonster(){
    float angle = -toAngle;
    
    if(angle < 0) angle = 360 + angle;
    
    if(angle == 360) angle = 0;
    CandyMonster::CandyType type = (CandyMonster::CandyType) (int)(angle/72 + 1);
    
    return type;
}

void MonsterSelector::addMonster(up<GameObject> monsterObj){
    float rads = 72.0f / 180 * PI * go->mChildren.size();
    float x = sinf(rads) * radius;
    float y = 0.0f;
    float z = cosf(rads) * radius;
    monsterObj->mTransform.translate( x, y, z);
    monsterObj->mTransform.rotate(180 + 72 * go->mChildren.size(), v3d(0, 1, 0));
    go->addChild(std::move(monsterObj));
}

void MonsterSelector::onTouchBegin(const int x, const int y){
    if(changeInProgress()) return;
    
    mPrevTouchPos = v2d(x, y);
    mCurrentTouchPos = v2d(x, y);
}

void MonsterSelector::onTouchMove(const int x, const int y){
    if(changeInProgress()) return;
    
    if(mPrevTouchPos.x == 0 && mPrevTouchPos.y == 0){
        mPrevTouchPos = v2d(x, y);
    }else{
        mCurrentTouchPos = v2d(x, y);
    }
}

void MonsterSelector::onTouchEnd(const int x, const int y){
    if(changeInProgress()) return;

    if(mPrevTouchPos == mCurrentTouchPos) return;
    
    if(mPrevTouchPos.x - mCurrentTouchPos.x > 0){   toAngle -= 72;
    }else{                                          toAngle += 72; }
    
    mPrevTouchPos = v2d(0, 0);
    mCurrentTouchPos = v2d(0, 0);
}

bool MonsterSelector::changeInProgress(){
    return (toAngle != currentAngle);
}