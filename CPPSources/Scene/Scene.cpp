#include "Scene.hpp"


static Scene   *mInstance = NULL;

Scene *Scene::instance(){
    if(!mInstance){
        mInstance = new Scene();
    }
    return mInstance;
}

Scene::Scene(){
    logMessage("Scene constructor!\n");
}

Scene::~Scene(){
    for(auto rootNode : mRoots){
        delete rootNode;
    }
    mRoots.clear();
    mInstance = nullptr;
    logMessage("Scene destructor!\n");
}

void Scene::addChild(ASceneNode *newNode){
    logMessage("Roots count: %d\t%d\n", mRoots.size(), newNode);
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