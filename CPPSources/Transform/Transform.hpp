#pragma once
#include <memory>
#include <vector>

#include "q4d.h"

class Transform  {

public:
    Transform();
    Transform(v3d& position);
    Transform(v3d&& position);
    
    ~Transform();
    
    const m4d& transformMatrix() const;
    void translate(float deltaX, float deltaY, float deltaZ);
    void translate(v3d tVector);
    void rotate(float xRad, float yRad, float zRad);
    void rotate(float deg, v3d axis);
    void rotate(q4d rotation);
    void scale(float xFactor, float yFactor, float zFactor);
    void scale(v3d& scaleVec);
    
    void moveForward(float velocity);
    void moveRight(float velocity);
    
    void refreshTransformMatrix();
    
    
    v3d mPosition{0, 0, 0};
    v3d mScale{1, 1, 1};
    q4d mRotation = q4d(0, 0, 0, 0);
    v3d mFront{0, 0, 1};
    v3d mUp{0, 1, 0};
    v3d mRight{1, 0, 0};
    
    static v3d baseFront;
    static v3d baseUp;

    
private:
    m4d mTransformMatrix;
};