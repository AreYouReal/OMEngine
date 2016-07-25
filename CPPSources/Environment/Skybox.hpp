#pragma once

#include "IComponent.hpp"
#include "GameObject.hpp"

class Skybox : public IComponent{
public:
    Skybox(GameObject * const gameObject);
    virtual         ~Skybox();
    
    virtual bool init() override;
    virtual void draw() override;
    
    static up<GameObject> create();
    
private:
    std::vector<sp<Texture>> textures;
    unsigned int vao;
    unsigned int vbo;
    unsigned int ibo;
    
    
    void initCubeTexturesAndMat();
    void initVAO();
};