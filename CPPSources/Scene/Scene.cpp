#include "Scene.hpp"


static Scene   *mInstance = NULL;

Scene *Scene::instance(){
    if(!mInstance){
        mInstance = new Scene();
    }
    return mInstance;
}

Scene::Scene(){}

Scene::~Scene(){
    for(auto rootNode : mRoots){
        delete rootNode;
    }
    mRoots.clear();
    if(mInstance){
        delete mInstance;
        mInstance = 0;
    }
}

void Scene::addChild(ASceneNode *newNode){
    mRoots.push_back(newNode);
}

void Scene::update(){
    for(auto rootNode : mRoots){
        rootNode->update();
    }

}

void Scene::setRenderObjectState(RenderObjectType newState){
    if(mDrawingState != newState) mDrawingState = newState;
}