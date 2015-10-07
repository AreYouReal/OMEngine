#include "Scene.hpp"

Scene::Scene(){
    logMessage("Scene constructor!\n");
}

Scene::~Scene(){
    mRoots.clear();
    logMessage("Scene destructor!\n");
}

void Scene::addChild(up<ASceneNode> newNode){
    mRoots.push_back(std::move(newNode));
}

void Scene::update(){
    for(auto const &rootNode : mRoots){
        rootNode->update();
    }

}

void Scene::setRenderObjectState(RenderObjectType newState){
    if(mDrawingState != newState) mDrawingState = newState;
}