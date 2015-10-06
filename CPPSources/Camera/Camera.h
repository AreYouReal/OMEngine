#pragma once

#include <memory>
#include <stack>

#include "Singleton.hpp"

#include "m4d.h"
#include "Transform.hpp"

class Camera : public Singleton<Camera>{
public:
    
    Camera(float fovy = 90, float width = 640, float height = 960, float near = 1, float far = 100);
    ~Camera();
    
    static void createCamera(float fovy = 90, float width = 640, float height = 960, float near = 0.1, float far = 100);

    const m4d& modelViewMatrix() const;
    const m4d& viewMatrix() const;
    const m4d& projectionMatrix() const;
    const m4d& normalMatrix() const;
    
    void setWidthAndHeight(float width, float height);
    void setPosition(v3d pos);
    void setFront(v3d front);
    void setUp(v3d up);
    
    void move(float velocity);
    void rotate(float angle, float x, float y, float z);
    void rotate(float angle, v3d& axis);
    
    float sphereDistanceInFrustum(v3d *location, float radius);
    
    void pushMVMatrix(m4d matrix);
    void popMVMatrix();
    
private:
    
    float mFovy, mWidth, mHeight, mNear, mFar;
    
    std::shared_ptr<Transform> transform;
    
    m4d mViewMatrix;
    m4d mProjectionMatrix;
    m4d mNormalMatrix;
    v4d frustum[6];
    
    std::stack<m4d> mMVstack;
    
    void refreshViewAndNormalMatrix();
    void refreshProjMatrix();
    void buildFrustum();
};