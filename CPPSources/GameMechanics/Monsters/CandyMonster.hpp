#pragma once

#include "IComponent.hpp"
#include "MeshRendererComponent.hpp"

class CandyMonster: public IComponent{
public:
    
    enum CandyType{ NONE = 0, TYPE_1 = 1, TYPE_2 = 2, TYPE_3 = 3, TYPE_4 = 4, TYPE_5 = 5  };
    
    CandyMonster(GameObject * const gameObject);
    virtual         ~CandyMonster();
    
    static up<GameObject> create(CandyType);
    
private:

};