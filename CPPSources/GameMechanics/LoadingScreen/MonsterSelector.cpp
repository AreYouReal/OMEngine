#include "MonsterSelector.hpp"
#include "Shortcuts.h"
#include "GameObject.hpp"


void MonsterSelector::update(){
    if(toAngle == currentAngle) return;

    currentAngle += 50.0f * Time::deltaTime;
    if(currentAngle > toAngle) currentAngle = toAngle;
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

void MonsterSelector::addMonster(up<GameObject> monsterObj){
    float rads = 72.0f / 180 * PI;
    v3d pos(
            cos(rads * go->mChildren.size() ) * radius,
            10.0f,
            sinf(rads * go->mChildren.size() ) * radius
    );
    monsterObj->mTransform.translate( pos );
    go->addChild(std::move(monsterObj));
    go->setPosition(v3d(0, 0, -7));
}

void MonsterSelector::onTouchBegin(const int x, const int y){
    toAngle += 72.0f;
}

void MonsterSelector::onTouchMove(const int x, const int y){

}

void MonsterSelector::onTouchEnd(const int x, const int y){

}
