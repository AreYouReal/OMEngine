#include "MonsterSelector.hpp"
#include "Shortcuts.h"
#include "GameObject.hpp"

void MonsterSelector::add(GameObject *const go){
    up<MonsterSelector> component = up<MonsterSelector>(new MonsterSelector(go));
    component->mComponentType = ComponentEnum::MONSTER_SELECTOR;
    go->addComponent(std::move(component));
}