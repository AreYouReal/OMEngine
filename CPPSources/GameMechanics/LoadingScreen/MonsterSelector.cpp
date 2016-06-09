#include "MonsterSelector.hpp"
#include "Shortcuts.h"
#include "GameObject.hpp"

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
}