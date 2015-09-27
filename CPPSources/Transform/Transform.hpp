#pragma once
#include <memory>
#include <vector>

#include "q4d.h"

class Transform{

public:
    Transform();
    Transform(v3d& position);
    
    const m4d& transformMatrix() const;
    void translate(float deltaX, float deltaY, float deltaZ);
    void translate(v3d tVector);
    void rotate(float xRad, float yRad, float zRad);
    void rotate(float rad, v3d axis);
    void scale(float xFactor, float yFactor, float zFactor);
    void scale(v3d& scaleVec);

    v3d mPosition;
    v3d mScale;
    q4d mRotation;
    v3d mFront;
    v3d mUp;
    
private:
    m4d mTransformMatrix;
    
    std::weak_ptr<Transform> parentTransform;
    std::vector<std::weak_ptr<Transform>> childTransforms;
    
    void refreshTransformMatrix();
};