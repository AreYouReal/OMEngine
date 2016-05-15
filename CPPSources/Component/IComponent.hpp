#pragma once

class GameObject;

enum class ComponentEnum{ MESH_RENDERER, RIGID_BODY, ANIM_MESH, DEBUG_DRAW, LEVEL_BUILDER, BBLOCK, ACTION_ARROW, PLAYER_CTR };

class IComponent{
public:
    
    GameObject      *go;

    IComponent(GameObject * const gameObject): go(gameObject){};
    virtual         ~IComponent(){};
    virtual bool    init()      {return true;};
    virtual void    update()    {};
    virtual void    draw()      {};
    virtual void    onDestroy() {};
};