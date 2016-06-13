#pragma once

#include "IComponent.hpp"
#include "Shortcuts.h"

class PlayButton : public IComponent{
public:
    
    PlayButton(GameObject * const gameObject);
    ~PlayButton(){};
    
    static up<GameObject> create();

};