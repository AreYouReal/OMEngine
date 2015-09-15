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
:mFovy(fovy), mWidth(width), mHeight(height), mNear(near), mFar(far){
    mViewMatrix = m4d::lookAt(mPosition, mFront, mUp);
    mProjectionMatrix = m4d::perspective(mFovy, mWidth, mHeight, mNear, mFar);
}


void Camera::setPosition(v3d pos){  mPosition = pos;    }
void Camera::setFront(v3d front){   mFront = front;     }
void Camera::setUp(v3d up){         mUp = up;           }

const m4d& Camera::viewMatrix() const{
    return mViewMatrix;
}

const m4d& Camera::projectionMatrix() const{
    return mProjectionMatrix;
}




