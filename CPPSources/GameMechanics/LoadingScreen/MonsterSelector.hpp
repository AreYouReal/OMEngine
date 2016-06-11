#pragma once

#include "IComponent.hpp"
#include "Shortcuts.h"

class MonsterSelector : public IComponent{
public:
    
    MonsterSelector(GameObject * const gameObject) : IComponent(gameObject){};
    virtual ~MonsterSelector(){};

    virtual void update() override;
    
    
    void addMonster(up<GameObject> monsterObj);
    
    static MonsterSelector* add(GameObject * const go);
    
    void onTouchBegin(const int x, const int y);
    void onTouchMove(const int x, const int y);
    void onTouchEnd(const int x, const int y);
    
private:
    float radius = 7.0f;

    float currentAngle = 0.0f;
    float toAngle = 0.0f;
    
    
};