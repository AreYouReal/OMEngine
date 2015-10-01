#include "Scene.hpp"


static Scene   *mInstance = NULL;

Scene *Scene::instance(){
    if(!mInstance){
        mInstance = new Scene();
    }
    return mInstance;
}

Scene::Scene(){
    mRoot = new ASceneNode(NULL, NULL);
}

Scene::~Scene(){
    if(mRoot){
        delete mRoot;
        mRoot = 0;
    }
    if(mInstance){
        delete mInstance;
        mInstance = 0;
    }
}

void Scene::addNode(ASceneNode *newNode){
    mRoot->addChild(newNode);
}

void Scene::update(){
    mRoot->update();
}