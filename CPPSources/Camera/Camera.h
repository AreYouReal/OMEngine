#pragma once

#include <memory>
#include "m4d.h"

class Camera{
public:
    static void createCamera(float fovy = 90, float width = 640, float height = 960, float near = 1, float far = 100);
    static std::shared_ptr<Camera> mCamera;
    static std::shared_ptr<Camera> instance();
    
    Camera(float fovy = 90, float width = 640, float height = 960, float near = 1, float far = 100);

    
private:
    float mFovy, mWidth, mHeight, mNear, mFar;
    
    v3d mPosition;
    v3d mFron;
    v3d mUp;
};