#include "Camera.h"

std::shared_ptr<Camera> Camera::mCamera = NULL;



void Camera::createCamera(float fovy, float width, float height, float near, float far){
    mCamera = std::shared_ptr<Camera>(new Camera(fovy, width, height, near, far));
}

std::shared_ptr<Camera> Camera::instance(){
    if(!mCamera){
        mCamera = std::shared_ptr<Camera>(new Camera());
    }
    return mCamera;
}

Camera::Camera(float fovy, float width, float height, float near, float far)
:mFovy(fovy), mWidth(width), mHeight(height), mNear(near), mFar(far){ }





