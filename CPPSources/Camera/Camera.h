#pragma once

#include <memory>
#include "m4d.h"
#include "Transform.hpp"

class Camera{
public:
    static void createCamera(float fovy = 90, float width = 640, float height = 960, float near = 0.1, float far = 100);
    static std::shared_ptr<Camera> mCamera;
    static std::shared_ptr<Camera> instance();

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
    
private:
    Camera(float fovy = 90, float width = 640, float height = 960, float near = 1, float far = 100);
    
    float mFovy, mWidth, mHeight, mNear, mFar;
    
    std::shared_ptr<Transform> transform;
    
    m4d mViewMatrix;
    m4d mProjectionMatrix;
    m4d mNormalMatrix;
    
    void refreshViewAndNormalMatrix();
    void refreshProjMatrix();
};