#pragma once

#include <memory>
#include "m4d.h"

class Camera{
public:
    static void createCamera(float fovy = 90, float width = 640, float height = 960, float near = 0.1, float far = 100);
    static std::shared_ptr<Camera> mCamera;
    static std::shared_ptr<Camera> instance();
    
    Camera(float fovy = 90, float width = 640, float height = 960, float near = 1, float far = 100);

    const m4d& viewMatrix() const;
    const m4d& projectionMatrix() const;
    
    void setWidthAndHeight(float width, float height);
    void setPosition(v3d pos);
    void setFront(v3d front);
    void setUp(v3d up);
    
    void move(bool forward);
    
private:
    float mFovy, mWidth, mHeight, mNear, mFar;
    
    v3d mPosition{0.0f, -2.0f, 0.0f};
    v3d mFront{0.0f, 0.0f, 0.0f};
    v3d mUp{0.0f, 0.0f, 1.0f};
    
    m4d mViewMatrix;
    m4d mProjectionMatrix;
    
    
    void refreshViewMatrix();
    void refreshProjMatrix();
};