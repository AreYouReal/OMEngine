#pragma once

#include <memory>
#include <stack>

#include "Singleton.hpp"

#include "m4d.h"
#include "Transform.hpp"

#include "btCollisionWorld.h"

class GameObject;
class Camera : public Singleton<Camera>{
public:
    
    Camera(float fovy = 80, float width = 640, float height = 960, float near = 0.1, float far = 100);
    Camera(const Camera& rhs) = delete;
    Camera& operator=(const Camera& rhs) = delete;
    ~Camera();
    
    const float width(){ return mWidth; }
    const float height(){ return mHeight; }
    
// Touch related
    void onTouchBegin   (const int x, const int y);
    void onTouchMove    (const int x, const int y);
    void onTouchEnd     (const int x, const int y);
//------------

    const m4d modelViewMatrix() const;
    const m4d& viewMatrix() const;
    const m4d& projectionMatrix() const;
    const m4d& normalMatrix() const;
    const m4d& orthoMatrix() const;
    
    void setWidthAndHeight(float width, float height);
    void setPosition(v3d pos);
    void setFront(v3d front);
    void setUp(v3d up);

    void follow(GameObject *go, v3d distance);
    
    void update();
    
    
    void rotate(float angle, float x, float y, float z);
    void rotate(float angle, v3d& axis);
    
    float sphereDistanceInFrustum(v3d location, float radius);
    
    void pushMMatrix(m4d matrix);
    void popMMatrix();

// Object picking related
    GameObject* collisionRayIntersection(int screenX, int screenY);
    

    
    
// Projector or shadows
    bool initShadowBuffer();
    
    const m4d& shadowMatrix() const;
    void refreshShadowMatrix();
    
    // Shadow related
    unsigned int shadowBuffer()     { return mShadowmapBuffer;  }
    unsigned int shadowmapWidth()   { return mShadowmapWidth;   }
    unsigned int shadowmapHeight()  { return mShadowmapHeight;  }
    unsigned int depthTexture()    { return mDepthTexture;    }
    unsigned int shadowTexture()    { return mShadowTexture;    }
    
    // Main render bufffer handle
    int mMainBuffer;
    
    bool shadowDraw = false;
    
private:
    
// Touch related variables
    v2d viewLocation, viewDelta{.0f, .0f};
    v3d moveLocation{.0f, .0f, .0f}, moveDelta;
//------------------------
    
    float mFovy, mWidth, mHeight, mNear, mFar;
    
    Transform transform;
    
    m4d mViewMatrix;
    m4d mProjectionMatrix;
    m4d mOrthoMatrix;
    m4d mNormalMatrix;
    v4d frustum[6];
    int viewportMatrix[4];
    
    std::stack<m4d> mMstack;
    
// Camera following
    GameObject *mGoToFollow;
    v3d         mFollowDistance;
    
// Projector 
    m4d mShadowMatrix;
    
    unsigned int mDepthTexture;
    unsigned int mShadowTexture;
    unsigned int mShadowmapBuffer;
    unsigned int mShadowmapWidth = 128;
    unsigned int mShadowmapHeight = 256;
    
    
    void refreshViewAndNormalMatrix();
    void refreshProjMatrix();
    void buildFrustum();
    
    v3d farPlanePoint(int screenX, int screenY);
};