#pragma once

#include "IComponent.hpp"
#include "Shortcuts.h"

class MonsterSelector : public IComponent{
public:
    
    MonsterSelector(GameObject * const gameObject) : IComponent(gameObject){};
    virtual ~MonsterSelector(){};

    void addMonster(up<GameObject> monsterObj);
    
    static MonsterSelector* add(GameObject * const go);

private:
    float radius = 10.0f;

};