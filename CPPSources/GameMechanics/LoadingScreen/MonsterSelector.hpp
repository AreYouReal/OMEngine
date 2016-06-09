#pragma once

#include "IComponent.hpp"

class MonsterSelector : public IComponent{
public:
        
    MonsterSelector(GameObject * const gameObject) : IComponent(gameObject){};
    virtual ~MonsterSelector(){};
  
    static void add(GameObject * const go);
    
};