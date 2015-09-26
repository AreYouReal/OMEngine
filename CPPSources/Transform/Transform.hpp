#pragma once

#include "q4d.h"

class Transform{

public:
    Transform();
    
    void setRotation(v3d& rotation);
    void setPosition(v3d& position);
    void setScale(v3d& scale);
    
    
    const m4d& transformMatrix() const;
    void translate(float deltaX, float deltaY, float deltaZ);
    void translate(v3d tVector);
    void rotateBy(float xRad, float yRad, float zRad);
    void rotateBy(float rad, v3d axis);
    void scale(float xFactor, float yFactor, float zFactor);
    
private:
    v3d mPosition;
    v3d mScale;
    q4d mRotation;
    m4d mTransformMatrix;
    
    void refreshTransformMatrix();
};