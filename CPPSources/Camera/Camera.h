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
    
    void update();
    
    // Getters
    const float width(){ return mWidth; }
    const float height(){ return mHeight; }

    const m4d modelViewMatrix() const;
    const m4d& viewMatrix() const;
    const m4d& projectionMatrix() const;
    const m4d& normalMatrix() const;
    const m4d& orthoMatrix() const;
    
    // Transform related
    void setWidthAndHeight(float width, float height);
    void setPosition(v3d pos);
    void setFront(v3d front);
    void setUp(v3d up);
    void follow(GameObject *go, v3d distance);
    
    // Moving related
    void moveTo(const v3d position, const float time);
    void rotateTo(float angle, v3d& axis);
    
// Matrix related
    void pushMMatrix(m4d matrix);
    void popMMatrix();

// Visibility check
    float sphereDistanceInFrustum(v3d location, float radius);
    
// Object picking related
    GameObject* collisionRayIntersection(int screenX, int screenY);
    
    // General drawing vars
    int     mMainBuffer;
    v4d     mClearColor{0.35f, 0.75f, 1.0f, 1.0f};
    
// Shadow related
    bool            shadowDraw = false;

    bool            initShadowBuffer();
    const m4d&      shadowMatrix() const;
    void            refreshShadowMatrix();
    unsigned int    shadowBuffer()     { return mShadowmapBuffer;  }
    unsigned int    shadowmapWidth()   { return mShadowmapWidth;   }
    unsigned int    shadowmapHeight()  { return mShadowmapHeight;  }
    unsigned int    depthTexture()     { return mDepthTexture;     }
    unsigned int    shadowTexture()    { return mShadowTexture;    }
    
private:
    
    Transform transform;    // Use mFront, mUp, mPosition for view matrix
    
    float mFovy, mWidth, mHeight, mNear, mFar;  // Projection matrix vars
    
    m4d mViewMatrix;
    m4d mProjectionMatrix;
    m4d mOrthoMatrix;
    m4d mNormalMatrix;
    v4d frustum[6];
    int viewportMatrix[4];
    
// Matrix stack
    std::stack<m4d> mMstack;
    
// Camera moving
    v3d         mMoveToPosition{9999, 9999, 9999};
    float       mMovingTime = 0.0f;
    
// Camera following
    GameObject *mGoToFollow = nullptr;
    v3d         mFollowDistance;
    
// Shadow related
    m4d             mShadowMatrix;
    unsigned int    mDepthTexture;
    unsigned int    mShadowTexture;
    unsigned int    mShadowmapBuffer;
    unsigned int    mShadowmapWidth = 128;
    unsigned int    mShadowmapHeight = 256;
    
// Helpers
    void refreshViewAndNormalMatrix();
    void refreshProjMatrix();
    void buildFrustum();
    
    v3d farPlanePoint(int screenX, int screenY);
    
    
// Moving related
    bool movingRoutine();
    bool followingRoutine();
};