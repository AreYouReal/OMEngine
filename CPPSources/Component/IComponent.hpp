#pragma once

class GameObject;

enum class ComponentEnum{ COMPONENT_ENUM_START, PLAY_BTN, MESH_RENDERER, MONSTER_SELECTOR, LIGHT_SOURCE, RIGID_BODY, ANIM_MESH, DEBUG_DRAW, LEVEL_BUILDER, BBLOCK, CANDY, PLAYER_CTR, SKYBOX, COMPONENT_ENUM_END };

class IComponent{
public:
    
    ComponentEnum   mComponentType;
    GameObject      *go;

    IComponent(GameObject * const gameObject): go(gameObject){};
    virtual         ~IComponent(){};
    virtual bool    init()      {return true;};
    virtual void    update()    {};
    virtual void    draw()      {};
    virtual void    onDestroy() {};
};