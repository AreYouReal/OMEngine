#include "Scene.hpp"

Scene::Scene(){
    logMessage("Scene constructor!\n");
}

Scene::~Scene(){
    mRoots.clear();
    logMessage("Scene destructor!\n");
}

void Scene::addObjOnScene(sp<IRenderable> newNode){
    mRoots.push_back(std::move(newNode));
}

void Scene::update(){
    for(auto const &rootNode : mRoots){
        rootNode->draw();
    }

}

void Scene::setRenderObjectState(RenderObjectType newState){
    if(mDrawingState != newState) mDrawingState = newState;
}