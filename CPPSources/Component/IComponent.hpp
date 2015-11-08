#pragma once

class GameObject;

enum class ComponentEnum{ MESH_RENDERER, RIGID_BODY };

class IComponent{
public:
    
    GameObject      *go;

    IComponent(GameObject * const gameObject): go(gameObject){};
    virtual         ~IComponent(){};
    virtual bool    init()          = 0;
    virtual void    update()        = 0;
    virtual void    onDestroy()     = 0;
};