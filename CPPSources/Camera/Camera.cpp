#include "Camera.h"

#include "SRUtils.h"

Camera *Camera::mInstance = NULL;

void Camera::createCamera(float fovy, float width, float height, float near, float far){
    mInstance = new Camera(fovy, width, height, near, far);
}

Camera* Camera::instance(){
    if(!mInstance){
        mInstance = new Camera();
    }
    return mInstance;
}

Camera::~Camera(){
    if(mInstance){
        delete mInstance;
        mInstance = 0;
    }
    logMessage("Camera destructor!");
}

Camera::Camera(float fovy, float width, float height, float near, float far)
:mFovy(fovy), mWidth(width), mHeight(height), mNear(near), mFar(far){
    transform = std::shared_ptr<Transform>(new Transform());
    transform->mPosition = v3d(0.0f, -3.8f, 2.5f);;
    transform->mFront = v3d(0.0f, 1.0f, 0.0f);;
    transform->mUp = v3d(0.0f, 0.0f, 1.0f);
    refreshViewAndNormalMatrix();
    refreshProjMatrix();
    logMessage("Camera constructor!");
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

const m4d& Camera::modelViewMatrix() const{
    if(mMVstack.empty()) return mViewMatrix;
    return mMVstack.top();
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

float Camera::sphereDistanceInFrustum(v3d *location, float radius){
    float d;
    for(unsigned char i = 0; i < 6; ++i){
        d = frustum[i].x * location->x + frustum[i].y * location->y + frustum[i].z * location->z + frustum[i].w;
        if(d < -radius) return 0.0f;
    }
    return d + radius;
}

#pragma mark PRIVATE(HELPERS)
void Camera::refreshViewAndNormalMatrix(){
    v3d lookAt = transform->mPosition + transform->mFront;
    mViewMatrix = m4d::lookAt(transform->mPosition, lookAt, transform->mUp);
    mNormalMatrix = m4d::inverseTranspose(mViewMatrix);
}
void Camera::refreshProjMatrix(){ mProjectionMatrix = m4d::perspective(mFovy, mWidth, mHeight, mNear, mFar);}

void Camera::buildFrustum(){
    m4d c;
    
    float t;
    
    c.m[ 0 ].x = mViewMatrix.m[ 0 ].x * mProjectionMatrix.m[ 0 ].x +
    mViewMatrix.m[ 0 ].y * mProjectionMatrix.m[ 1 ].x +
    mViewMatrix.m[ 0 ].z * mProjectionMatrix.m[ 2 ].x +
    mViewMatrix.m[ 0 ].w * mProjectionMatrix.m[ 3 ].x;
    
    c.m[ 0 ].y = mViewMatrix.m[ 0 ].x * mProjectionMatrix.m[ 0 ].y +
    mViewMatrix.m[ 0 ].y * mProjectionMatrix.m[ 1 ].y +
    mViewMatrix.m[ 0 ].z * mProjectionMatrix.m[ 2 ].y +
    mViewMatrix.m[ 0 ].w * mProjectionMatrix.m[ 3 ].y;
    
    c.m[ 0 ].z = mViewMatrix.m[ 0 ].x * mProjectionMatrix.m[ 0 ].z +
    mViewMatrix.m[ 0 ].y * mProjectionMatrix.m[ 1 ].z +
    mViewMatrix.m[ 0 ].z * mProjectionMatrix.m[ 2 ].z +
    mViewMatrix.m[ 0 ].w * mProjectionMatrix.m[ 3 ].z;
    
    c.m[ 0 ].w = mViewMatrix.m[ 0 ].x * mProjectionMatrix.m[ 0 ].w +
    mViewMatrix.m[ 0 ].y * mProjectionMatrix.m[ 1 ].w +
    mViewMatrix.m[ 0 ].z * mProjectionMatrix.m[ 2 ].w +
    mViewMatrix.m[ 0 ].w * mProjectionMatrix.m[ 3 ].w;
    
    c.m[ 1 ].x = mViewMatrix.m[ 1 ].x * mProjectionMatrix.m[ 0 ].x +
    mViewMatrix.m[ 1 ].y * mProjectionMatrix.m[ 1 ].x +
    mViewMatrix.m[ 1 ].z * mProjectionMatrix.m[ 2 ].x +
    mViewMatrix.m[ 1 ].w * mProjectionMatrix.m[ 3 ].x;
    
    c.m[ 1 ].y = mViewMatrix.m[ 1 ].x * mProjectionMatrix.m[ 0 ].y +
    mViewMatrix.m[ 1 ].y * mProjectionMatrix.m[ 1 ].y +
    mViewMatrix.m[ 1 ].z * mProjectionMatrix.m[ 2 ].y +
    mViewMatrix.m[ 1 ].w * mProjectionMatrix.m[ 3 ].y;
    
    c.m[ 1 ].z = mViewMatrix.m[ 1 ].x * mProjectionMatrix.m[ 0 ].z +
    mViewMatrix.m[ 1 ].y * mProjectionMatrix.m[ 1 ].z +
    mViewMatrix.m[ 1 ].z * mProjectionMatrix.m[ 2 ].z +
    mViewMatrix.m[ 1 ].w * mProjectionMatrix.m[ 3 ].z;
    
    c.m[ 1 ].w = mViewMatrix.m[ 1 ].x * mProjectionMatrix.m[ 0 ].w +
    mViewMatrix.m[ 1 ].y * mProjectionMatrix.m[ 1 ].w +
    mViewMatrix.m[ 1 ].z * mProjectionMatrix.m[ 2 ].w +
    mViewMatrix.m[ 1 ].w * mProjectionMatrix.m[ 3 ].w;
    
    c.m[ 2 ].x = mViewMatrix.m[ 2 ].x * mProjectionMatrix.m[ 0 ].x +
    mViewMatrix.m[ 2 ].y * mProjectionMatrix.m[ 1 ].x +
    mViewMatrix.m[ 2 ].z * mProjectionMatrix.m[ 2 ].x +
    mViewMatrix.m[ 2 ].w * mProjectionMatrix.m[ 3 ].x;
    
    c.m[ 2 ].y = mViewMatrix.m[ 2 ].x * mProjectionMatrix.m[ 0 ].y +
    mViewMatrix.m[ 2 ].y * mProjectionMatrix.m[ 1 ].y +
    mViewMatrix.m[ 2 ].z * mProjectionMatrix.m[ 2 ].y +
    mViewMatrix.m[ 2 ].w * mProjectionMatrix.m[ 3 ].y;
    
    c.m[ 2 ].z = mViewMatrix.m[ 2 ].x * mProjectionMatrix.m[ 0 ].z +
    mViewMatrix.m[ 2 ].y * mProjectionMatrix.m[ 1 ].z +
    mViewMatrix.m[ 2 ].z * mProjectionMatrix.m[ 2 ].z +
    mViewMatrix.m[ 2 ].w * mProjectionMatrix.m[ 3 ].z;
    
    c.m[ 2 ].w = mViewMatrix.m[ 2 ].x * mProjectionMatrix.m[ 0 ].w +
    mViewMatrix.m[ 2 ].y * mProjectionMatrix.m[ 1 ].w +
    mViewMatrix.m[ 2 ].z * mProjectionMatrix.m[ 2 ].w +
    mViewMatrix.m[ 2 ].w * mProjectionMatrix.m[ 3 ].w;
    
    c.m[ 3 ].x = mViewMatrix.m[ 3 ].x * mProjectionMatrix.m[ 0 ].x +
    mViewMatrix.m[ 3 ].y * mProjectionMatrix.m[ 1 ].x +
    mViewMatrix.m[ 3 ].z * mProjectionMatrix.m[ 2 ].x +
    mViewMatrix.m[ 3 ].w * mProjectionMatrix.m[ 3 ].x;
    
    c.m[ 3 ].y = mViewMatrix.m[ 3 ].x * mProjectionMatrix.m[ 0 ].y +
    mViewMatrix.m[ 3 ].y * mProjectionMatrix.m[ 1 ].y +
    mViewMatrix.m[ 3 ].z * mProjectionMatrix.m[ 2 ].y +
    mViewMatrix.m[ 3 ].w * mProjectionMatrix.m[ 3 ].y;
    
    c.m[ 3 ].z = mViewMatrix.m[ 3 ].x * mProjectionMatrix.m[ 0 ].z +
    mViewMatrix.m[ 3 ].y * mProjectionMatrix.m[ 1 ].z +
    mViewMatrix.m[ 3 ].z * mProjectionMatrix.m[ 2 ].z +
    mViewMatrix.m[ 3 ].w * mProjectionMatrix.m[ 3 ].z;
    
    c.m[ 3 ].w = mViewMatrix.m[ 3 ].x * mProjectionMatrix.m[ 0 ].w +
    mViewMatrix.m[ 3 ].y * mProjectionMatrix.m[ 1 ].w +
    mViewMatrix.m[ 3 ].z * mProjectionMatrix.m[ 2 ].w +
    mViewMatrix.m[ 3 ].w * mProjectionMatrix.m[ 3 ].w;
    
    
    frustum[ 0 ].x = c.m[ 0 ].w - c.m[ 0 ].x ;
    frustum[ 0 ].y = c.m[ 1 ].w - c.m[ 1 ].x ;
    frustum[ 0 ].z = c.m[ 2 ].w - c.m[ 2 ].x ;
    frustum[ 0 ].w = c.m[ 3 ].w - c.m[ 3 ].x ;
    
    t = 1.0f / sqrtf( frustum[ 0 ].x * frustum[ 0 ].x +
                     frustum[ 0 ].y * frustum[ 0 ].y +
                     frustum[ 0 ].z * frustum[ 0 ].z );
    
    frustum[ 0 ].x *= t;
    frustum[ 0 ].y *= t;
    frustum[ 0 ].z *= t;
    frustum[ 0 ].w *= t;
    
    
    frustum[ 1 ].x = c.m[ 0 ].w + c.m[ 0 ].x ;
    frustum[ 1 ].y = c.m[ 1 ].w + c.m[ 1 ].x ;
    frustum[ 1 ].z = c.m[ 2 ].w + c.m[ 2 ].x ;
    frustum[ 1 ].w = c.m[ 3 ].w + c.m[ 3 ].x ;
    
    t = 1.0f / sqrtf( frustum[ 1 ].x * frustum[ 1 ].x +
                     frustum[ 1 ].y * frustum[ 1 ].y +
                     frustum[ 1 ].z * frustum[ 1 ].z );
    
    frustum[ 1 ].x *= t;
    frustum[ 1 ].y *= t;
    frustum[ 1 ].z *= t;
    frustum[ 1 ].w *= t;
    
    
    frustum[ 2 ].x = c.m[ 0 ].w + c.m[ 0 ].y ;
    frustum[ 2 ].y = c.m[ 1 ].w + c.m[ 1 ].y ;
    frustum[ 2 ].z = c.m[ 2 ].w + c.m[ 2 ].y ;
    frustum[ 2 ].w = c.m[ 3 ].w + c.m[ 3 ].y ;
    
    t = 1.0f / sqrtf( frustum[ 2 ].x * frustum[ 2 ].x +
                     frustum[ 2 ].y * frustum[ 2 ].y +
                     frustum[ 2 ].z * frustum[ 2 ].z );
    
    frustum[ 2 ].x *= t;
    frustum[ 2 ].y *= t;
    frustum[ 2 ].z *= t;
    frustum[ 2 ].w *= t;
    
    
    frustum[ 3 ].x = c.m[ 0 ].w - c.m[ 0 ].y ;
    frustum[ 3 ].y = c.m[ 1 ].w - c.m[ 1 ].y ;
    frustum[ 3 ].z = c.m[ 2 ].w - c.m[ 2 ].y ;
    frustum[ 3 ].w = c.m[ 3 ].w - c.m[ 3 ].y ;
    
    t = 1.0f / sqrtf( frustum[ 3 ].x * frustum[ 3 ].x +
                     frustum[ 3 ].y * frustum[ 3 ].y +
                     frustum[ 3 ].z * frustum[ 3 ].z );
    
    frustum[ 3 ].x *= t;
    frustum[ 3 ].y *= t;
    frustum[ 3 ].z *= t;
    frustum[ 3 ].w *= t;
    
    
    frustum[ 4 ].x = c.m[ 0 ].w - c.m[ 0 ].z ;
    frustum[ 4 ].y = c.m[ 1 ].w - c.m[ 1 ].z ;
    frustum[ 4 ].z = c.m[ 2 ].w - c.m[ 2 ].z ;
    frustum[ 4 ].w = c.m[ 3 ].w - c.m[ 3 ].z ;
    
    t = 1.0f / sqrtf( frustum[ 4 ].x * frustum[ 4 ].x +
                     frustum[ 4 ].y * frustum[ 4 ].y +
                     frustum[ 4 ].z * frustum[ 4 ].z );
    
    frustum[ 4 ].x *= t;
    frustum[ 4 ].y *= t;
    frustum[ 4 ].z *= t;
    frustum[ 4 ].w *= t;
    
    
    frustum[ 5 ].x = c.m[ 0 ].w + c.m[ 0 ].z ;
    frustum[ 5 ].y = c.m[ 1 ].w + c.m[ 1 ].z ;
    frustum[ 5 ].z = c.m[ 2 ].w + c.m[ 2 ].z ;
    frustum[ 5 ].w = c.m[ 3 ].w + c.m[ 3 ].z ;
    
    
    t = 1.0f / sqrtf( frustum[ 5 ].x * frustum[ 5 ].x +
                     frustum[ 5 ].y * frustum[ 5 ].y +
                     frustum[ 5 ].z * frustum[ 5 ].z );
    
    frustum[ 5 ].x *= t;
    frustum[ 5 ].y *= t;
    frustum[ 5 ].z *= t;
    frustum[ 5 ].w *= t;
}

void Camera::pushMVMatrix(m4d matrix){
    mMVstack.push(matrix);
}

void Camera::popMVMatrix(){
    return mMVstack.pop();
}





