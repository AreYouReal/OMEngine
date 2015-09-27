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
    transform->mPosition = v3d(0.0f, -3.8f, 2.5f);;
    transform->mFront = v3d(0.0f, 1.0f, 0.0f);;
    transform->mUp = v3d(0.0f, 0.0f, 1.0f);
    mViewMatrix = m4d::lookAt(transform->mPosition, transform->mFront, transform->mUp);
    mProjectionMatrix = m4d::perspective(mFovy, mWidth, mHeight, mNear, mFar);
    mNormalMatrix = m4d::inverseTranspose(mViewMatrix);
}


void Camera::setPosition(v3d pos){
    transform->mPosition = pos;
    refreshViewAndNormalMatrix();
}
void Camera::setFront(v3d front){
    transform->mFront = front;
    refreshViewAndNormalMatrix();
}
void Camera::setUp(v3d up){
    transform->mUp = up;
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

void Camera::move(float velocity){
    transform->moveForward(velocity);
    refreshViewAndNormalMatrix();
}

void Camera::rotate(float angle, float x, float y, float z){
    v3d axis(x, y, z);
    transform->rotate(angle, axis);
    refreshViewAndNormalMatrix();
}

void Camera::rotate(float deg, v3d &axis){
    transform->rotate(deg, axis);
    refreshViewAndNormalMatrix();
}

#pragma mark PRIVATE(HELPERS)
inline void Camera::refreshViewAndNormalMatrix(){
    mViewMatrix = m4d::lookAt(transform->mPosition, transform->mPosition + transform->mFront, transform->mUp);
    mNormalMatrix = m4d::inverseTranspose(mViewMatrix);
}
inline void Camera::refreshProjMatrix(){ mProjectionMatrix = m4d::perspective(mFovy, mWidth, mHeight, mNear, mFar);}



