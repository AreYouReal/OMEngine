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
    transform = std::shared_ptr<Transform>(new Transform());
    v3d pos = v3d(0.0f, -3.8f, 2.5f);
    v3d front = v3d(0.0f, 1.0f, 0.0f);
    v3d up = v3d(0.0f, 0.0f, 1.0f);
    transform->setPosition(pos);
    transform->setFront(front);
    transform->setUp(up);
    mViewMatrix = m4d::lookAt(transform->position(), transform->front(), transform->up());
    mProjectionMatrix = m4d::perspective(mFovy, mWidth, mHeight, mNear, mFar);
    mNormalMatrix = m4d::inverseTranspose(mViewMatrix);
}


void Camera::setPosition(v3d pos){
    transform->setPosition(pos);
    refreshViewAndNormalMatrix();
}
void Camera::setFront(v3d front){
    transform->setFront(front);
    refreshViewAndNormalMatrix();
}
void Camera::setUp(v3d up){
    transform->setUp(up);
    refreshViewAndNormalMatrix();
}
void Camera::setWidthAndHeight(float width, float height){
    if(mWidth == width && mHeight == height) return;
    mWidth = width;
    mHeight = height;
    refreshProjMatrix();
}

const m4d& Camera::viewMatrix() const{
    return mViewMatrix;
}

const m4d& Camera::projectionMatrix() const{
    return mProjectionMatrix;
}

const m4d& Camera::normalMatrix() const{
    return mNormalMatrix;
}

void Camera::move(bool forward){
    if(forward){
        transform->translate(transform->front());
    }else transform->translate(-transform->front());
    refreshViewAndNormalMatrix();
}

#pragma mark PRIVATE(HELPERS)
inline void Camera::refreshViewAndNormalMatrix(){
    mViewMatrix = m4d::lookAt(transform->position(), transform->front(), transform->up());
    mNormalMatrix = m4d::inverseTranspose(mViewMatrix);
}
inline void Camera::refreshProjMatrix(){ mProjectionMatrix = m4d::perspective(mFovy, mWidth, mHeight, mNear, mFar);}



