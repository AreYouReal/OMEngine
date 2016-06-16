#include "Camera.h"

#include "OMUtils.h"

#include "Illuminator.hpp"
#include "GameObject.hpp"


#define CLAMP(x, min, max) ((x < min) ? min : ((x > max) ? max : x));

#define SIGN(x) (x >= 0 ? -1 : 1);

Camera::Camera(float fovy, float width, float height, float near, float far)
:mFovy(fovy), mWidth(width), mHeight(height), mNear(near), mFar(far){
transform.mPosition = v3d(8.0f, 5.0f, -6.0f);
refreshViewAndNormalMatrix();
refreshProjMatrix();
}

bool Camera::initShadowBuffer(){

    GLenum none = GL_NONE;
    GLint defaultFramebuffer = 0;
    
//    glGenTextures ( 1, &mShadowTexture );
//    glBindTexture ( GL_TEXTURE_2D, mShadowTexture );
//    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
//    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
//    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE );
//    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE );
    

    
//    glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB,
//                  mShadowmapWidth, mShadowmapHeight,
//                  0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL );
    
    
    glGenTextures ( 1, &mDepthTexture );
    glBindTexture ( GL_TEXTURE_2D, mDepthTexture );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE );
    
    // Setup hardware comparison
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL );
    
    glTexImage2D ( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24,
                  mShadowmapWidth, mShadowmapHeight,
                  0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL );
    
    glBindTexture ( GL_TEXTURE_2D, 0 );
    
    glGetIntegerv ( GL_FRAMEBUFFER_BINDING, &defaultFramebuffer );
    
    // setup fbo
    glGenFramebuffers ( 1, &mShadowmapBuffer );
    glBindFramebuffer ( GL_FRAMEBUFFER, mShadowmapBuffer );
    
    glDrawBuffers ( 1, &none );
    
    glFramebufferTexture2D ( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture, 0 );
    
//    glFramebufferTexture2D ( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mShadowTexture, 0 );
    
    glActiveTexture ( GL_TEXTURE0 );
    glBindTexture ( GL_TEXTURE_2D, mDepthTexture );
    
    if ( GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus ( GL_FRAMEBUFFER ) ){
        return false;
    }else{
//        logMessage("FRAMEBUFFER COMPLETE!!!\n");
    }
    
    glBindFramebuffer ( GL_FRAMEBUFFER, defaultFramebuffer );

    logGLError();
    
    return true;
}

Camera::~Camera(){}

void Camera::draw(){
    glBindFramebuffer(GL_FRAMEBUFFER, mMainBuffer);
    glColorMask ( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
    glViewport(0, 0, mWidth, mHeight);
    glClearColor(mClearColor.x, mClearColor.y, mClearColor.z, mClearColor.w);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
}

void Camera::update(){
    if(mMoveToPosition.x != 9999){
        movingRoutine();
    }else{
        followingRoutine();
    }
    
    if(mLookAtAim.x != 9999){
        lookAtRoutine();
    }
    
    refreshViewAndNormalMatrix();
    refreshShadowMatrix();
}

void Camera::setWidthAndHeight(float width, float height){
    if(mWidth == width && mHeight == height && viewportMatrix[2] != 0 && viewportMatrix[3] != 0) return;
    mWidth = width;
    mHeight = height;
    refreshProjMatrix();
    glGetIntegerv(GL_VIEWPORT, viewportMatrix);
}

#pragma Transform related
void Camera::moveTo(const v3d position, const float time){
    mMoveToPosition = position;
    mMovingTime = time;
}

void Camera::setPosition(v3d pos){
    transform.mPosition = pos;
    refreshViewAndNormalMatrix();
}
void Camera::lookAt(v3d front, const float time){
    if(time <= 0.0f){
        mLookAt = front;
        transform.mFront = front - transform.mPosition;
    }else{
        mLookAtAim = front;
        mLookAtTime = time;
    }
    refreshViewAndNormalMatrix();
}
void Camera::setUp(v3d up){
    transform.mUp = up;
    refreshViewAndNormalMatrix();
}

void Camera::follow(GameObject *go, v3d distance){
    mGoToFollow = go;
    mFollowDistance = distance;
}

#pragma Visibility Check related
float Camera::sphereDistanceInFrustum(v3d location, float radius){
    float d;
    for(unsigned int i = 0; i < 6; ++i){
        d = frustum[i].x * location.x + frustum[i].y * location.y + frustum[i].z * location.z + frustum[i].w;
        if(d < -radius){
            return 0.0f;
        }
    }
    return d + radius;
}

#pragma Matrix getters
const m4d& Camera::viewMatrix() const{
    return mViewMatrix;
}

const m4d& Camera::projectionMatrix() const{
    return mProjectionMatrix;
}

const m4d& Camera::normalMatrix() const{
    return mNormalMatrix;
}

const m4d& Camera::orthoMatrix() const{
    return mOrthoMatrix;
}

const m4d& Camera::shadowMatrix() const{
    return mShadowMatrix;
}

const m4d Camera::modelViewMatrix() const{
    if(mMstack.empty()){
        if(shadowDraw){
            return Illuminator::instance()->getLightSource()->getLookAtFromPointView() * mMstack.top();
        }else{
            return mViewMatrix;
        }
    }else{
        if(shadowDraw){
            return Illuminator::instance()->getLightSource()->getLookAtFromPointView() * mMstack.top();
        }else{
            return mViewMatrix * mMstack.top();
        }
    }

}

void Camera::pushMMatrix(m4d matrix){
    if(mMstack.empty()) mMstack.push(matrix);
    else mMstack.push(mMstack.top() * matrix);
}

void Camera::popMMatrix(){
    return mMstack.pop();
}

#pragma mark PRIVATE(HELPERS)
void Camera::refreshViewAndNormalMatrix(){
    v3d lookAt = transform.mPosition + transform.mFront;
    mViewMatrix = m4d::lookAt(transform.mPosition, lookAt, transform.mUp);
    mNormalMatrix = m4d::inverseTranspose(mViewMatrix);
    buildFrustum();
}
void Camera::refreshProjMatrix(){
    mProjectionMatrix = m4d::perspective(mFovy, mWidth, mHeight, mNear, mFar);
    mOrthoMatrix = m4d::ortho(-mWidth * 0.5f, mWidth * 0.5f, -mHeight * 0.5, mHeight * 0.5, -1, 1);
    buildFrustum();
}

void Camera::refreshShadowMatrix(){
    LightSource* light = Illuminator::instance()->getLightSource();
    m4d perspective = m4d::perspective(70, mWidth, mHeight, mNear, mFar);

    m4d lookAt = light->getLookAtFromPointView();
    
    mShadowMatrix.m[0].x = 0.5f;
    mShadowMatrix.m[0].y = 0.0f;
    mShadowMatrix.m[0].z = 0.0f;
    mShadowMatrix.m[0].w = 0.0f;
    
    mShadowMatrix.m[1].x = 0.0f;
    mShadowMatrix.m[1].y = 0.5f;
    mShadowMatrix.m[1].z = 0.0f;
    mShadowMatrix.m[1].w = 0.0f;
    
    mShadowMatrix.m[2].x = 0.0f;
    mShadowMatrix.m[2].y = 0.0f;
    mShadowMatrix.m[2].z = 0.5f;
    mShadowMatrix.m[2].w = 0.0f;
    
    mShadowMatrix.m[3].x = 0.5f;
    mShadowMatrix.m[3].y = 0.5f;
    mShadowMatrix.m[3].z = 0.5f;
    mShadowMatrix.m[3].w = 1.0f;
    
    mShadowMatrix = m4d::transpose(mShadowMatrix) * perspective * lookAt;
    // Do not forget translate this matrix to object position before drawing.
}

void Camera::buildFrustum(){
    m4d c;
    
    float t;
    
    c.m[ 0 ].x = mViewMatrix.m[ 0 ].x * mProjectionMatrix.m[ 0 ].x +
    mViewMatrix.m[ 1 ].x * mProjectionMatrix.m[ 0 ].y +
    mViewMatrix.m[ 2 ].x * mProjectionMatrix.m[ 0 ].z +
    mViewMatrix.m[ 3 ].x * mProjectionMatrix.m[ 0 ].w;
    
    
    c.m[ 0 ].y = mViewMatrix.m[ 0 ].x * mProjectionMatrix.m[ 1 ].x +
    mViewMatrix.m[ 1 ].x * mProjectionMatrix.m[ 1 ].y +
    mViewMatrix.m[ 2 ].x * mProjectionMatrix.m[ 1 ].z +
    mViewMatrix.m[ 3 ].x * mProjectionMatrix.m[ 1 ].w;
    
    
    c.m[ 0 ].z = mViewMatrix.m[ 0 ].x * mProjectionMatrix.m[ 2 ].x +
    mViewMatrix.m[ 1 ].x * mProjectionMatrix.m[ 2 ].y +
    mViewMatrix.m[ 2 ].x * mProjectionMatrix.m[ 2 ].z +
    mViewMatrix.m[ 3 ].x * mProjectionMatrix.m[ 2 ].w;
    
    
    c.m[ 0 ].w = mViewMatrix.m[ 0 ].x * mProjectionMatrix.m[ 3 ].x +
    mViewMatrix.m[ 1 ].x * mProjectionMatrix.m[ 3 ].y +
    mViewMatrix.m[ 2 ].x * mProjectionMatrix.m[ 3 ].z +
    mViewMatrix.m[ 3 ].x * mProjectionMatrix.m[ 3 ].w;
    
    c.m[ 1 ].x = mViewMatrix.m[ 0 ].y * mProjectionMatrix.m[ 0 ].x +
    mViewMatrix.m[ 1 ].y * mProjectionMatrix.m[ 0 ].y +
    mViewMatrix.m[ 2 ].y * mProjectionMatrix.m[ 0 ].z +
    mViewMatrix.m[ 3 ].y * mProjectionMatrix.m[ 0 ].w;
    
    
    c.m[ 1 ].y = mViewMatrix.m[ 0 ].y * mProjectionMatrix.m[ 1 ].x +
    mViewMatrix.m[ 1 ].y * mProjectionMatrix.m[ 1 ].y +
    mViewMatrix.m[ 2 ].y * mProjectionMatrix.m[ 1 ].z +
    mViewMatrix.m[ 3 ].y * mProjectionMatrix.m[ 1 ].w;
    
    
    
    c.m[ 1 ].z = mViewMatrix.m[ 0 ].y * mProjectionMatrix.m[ 2 ].x +
    mViewMatrix.m[ 1 ].y * mProjectionMatrix.m[ 2 ].y +
    mViewMatrix.m[ 2 ].y * mProjectionMatrix.m[ 2 ].z +
    mViewMatrix.m[ 3 ].y * mProjectionMatrix.m[ 2 ].w;
    
    
    
    c.m[ 1 ].w = mViewMatrix.m[ 0 ].y * mProjectionMatrix.m[ 3 ].x +
    mViewMatrix.m[ 1 ].y * mProjectionMatrix.m[ 3 ].y +
    mViewMatrix.m[ 2 ].y * mProjectionMatrix.m[ 3 ].z +
    mViewMatrix.m[ 3 ].y * mProjectionMatrix.m[ 3 ].w;
    
    
    
    c.m[ 2 ].x = mViewMatrix.m[ 0 ].z * mProjectionMatrix.m[ 0 ].x +
    mViewMatrix.m[ 1 ].z * mProjectionMatrix.m[ 0 ].y +
    mViewMatrix.m[ 2 ].z * mProjectionMatrix.m[ 0 ].z +
    mViewMatrix.m[ 3 ].z * mProjectionMatrix.m[ 0 ].w;
    
    c.m[ 2 ].y = mViewMatrix.m[ 0 ].z * mProjectionMatrix.m[ 1 ].x +
    mViewMatrix.m[ 1 ].z * mProjectionMatrix.m[ 1 ].y +
    mViewMatrix.m[ 2 ].z * mProjectionMatrix.m[ 1 ].z +
    mViewMatrix.m[ 3 ].z * mProjectionMatrix.m[ 1 ].w;
    
    c.m[ 2 ].z = mViewMatrix.m[ 0 ].z * mProjectionMatrix.m[ 2 ].x +
    mViewMatrix.m[ 1 ].z * mProjectionMatrix.m[ 2 ].y +
    mViewMatrix.m[ 2 ].z * mProjectionMatrix.m[ 2 ].z +
    mViewMatrix.m[ 3 ].z * mProjectionMatrix.m[ 2 ].w;
    
    c.m[ 2 ].w = mViewMatrix.m[ 0 ].z * mProjectionMatrix.m[ 3 ].x +
    mViewMatrix.m[ 1 ].z * mProjectionMatrix.m[ 3 ].y +
    mViewMatrix.m[ 2 ].z * mProjectionMatrix.m[ 3 ].z +
    mViewMatrix.m[ 3 ].z * mProjectionMatrix.m[ 3 ].w;
    
    c.m[ 3 ].x = mViewMatrix.m[ 0 ].w * mProjectionMatrix.m[ 0 ].x +
    mViewMatrix.m[ 1 ].w * mProjectionMatrix.m[ 0 ].y +
    mViewMatrix.m[ 2 ].w * mProjectionMatrix.m[ 0 ].z +
    mViewMatrix.m[ 3 ].w * mProjectionMatrix.m[ 0 ].w;
    
    c.m[ 3 ].y = mViewMatrix.m[ 0 ].w * mProjectionMatrix.m[ 1 ].x +
    mViewMatrix.m[ 1 ].w * mProjectionMatrix.m[ 1 ].y +
    mViewMatrix.m[ 2 ].w * mProjectionMatrix.m[ 1 ].z +
    mViewMatrix.m[ 3 ].w * mProjectionMatrix.m[ 1 ].w;
    
    c.m[ 3 ].z = mViewMatrix.m[ 0 ].w * mProjectionMatrix.m[ 2 ].x +
    mViewMatrix.m[ 1 ].w * mProjectionMatrix.m[ 2 ].y +
    mViewMatrix.m[ 2 ].w * mProjectionMatrix.m[ 2 ].z +
    mViewMatrix.m[ 3 ].w * mProjectionMatrix.m[ 2 ].w;
    
    c.m[ 3 ].w = mViewMatrix.m[ 0 ].w * mProjectionMatrix.m[ 3 ].x +
    mViewMatrix.m[ 1 ].w * mProjectionMatrix.m[ 3 ].y +
    mViewMatrix.m[ 2 ].w * mProjectionMatrix.m[ 3 ].z +
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

#pragma Camera Raycasting related
GameObject* Camera::collisionRayIntersection(int screenX, int screenY){
    v3d fpp = farPlanePoint(screenX, screenY);
    btVector3 from(transform.mPosition.x, transform.mPosition.y, transform.mPosition.z);
    btVector3 to(fpp.x, fpp.y, fpp.z);
    btCollisionWorld::ClosestRayResultCallback collisionRay(from, to);
    PhysicalWorld::instance()->pWorld()->rayTest(from, to, collisionRay);
    if(collisionRay.hasHit()){
        return ((GameObject*)collisionRay.m_collisionObject->getUserPointer());
    }else{
        return nullptr;
    }
}

v3d Camera::farPlanePoint(int screenX, int screenY){
    m4d unprojMatrix = m4d::inverse( mProjectionMatrix * mViewMatrix );
    
#ifdef ANDROID

#else
#ifdef __APPLE__

#endif
    mWidth = 320; mHeight = 480;
#endif
    float x = screenX;
    float y = screenY;
    
    y = mHeight - y;
    
    x = (x) / mWidth;
    y = (y) / mHeight;
    x = x * 2.0f - 1.0f;
    y = y * 2.0f - 1.0f;
    
    v4d screenVec(x, y, 1.0f, 1.0f);
 
    v4d farPlanevec = screenVec * unprojMatrix;
    
    return v3d(farPlanevec);
}

void Camera::movingRoutine(){
    if(mMoveToPosition.x != 9999){
        static float moveTime = mMovingTime;
        if(moveTime <= 0.0f){
            transform.mPosition = mMoveToPosition;
            mMoveToPosition = v3d(9999, 9999, 9999);
            mMovingTime = -1.0f;
        }else{
            float percent = (mMovingTime - moveTime) / mMovingTime;
            v3d::print(transform.mPosition);
            transform.mPosition = v3d::lerp(transform.mPosition, mMoveToPosition, percent * Time::deltaTime);
        }
        moveTime -= Time::deltaTime;
    }
}

void Camera::followingRoutine(){
    if(mGoToFollow){
        transform.mPosition = v3d::lerp(transform.mPosition, mGoToFollow->getPosition() + mFollowDistance, Time::deltaTime );
        transform.mFront = mGoToFollow->getPosition() - transform.mPosition;
    }
}

void Camera::lookAtRoutine(){
    if(mLookAtAim.x != 9999){
        static float time = mLookAtTime;
        if(time <= 0.0f){
            transform.mFront = (mLookAtAim - transform.mPosition).normalize();
            mLookAtAim = v3d(9999, 9999, 9999);
            mLookAtTime = -1.0f;
        }else{
            float percent = (mLookAtTime - time) / mLookAtTime;
            transform.mFront =  (v3d::lerp(mLookAt, mLookAtAim, percent ) - transform.mPosition).normalize();
        }
        time -= Time::deltaTime;
    }
}