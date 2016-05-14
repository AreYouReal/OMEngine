#pragma once

#include "GameObject.hpp"

class BBlock{
public:
    BBlock(GameObject *go);
    ~BBlock();
    
    void reinit(v3d newPos);
    void show();
    void hide();
    
private:
    GameObject *mGo;
    
    
};