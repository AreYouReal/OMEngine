#include "Transform.hpp"


Transform::Transform() : mPosition(v3d(0.0f, 0.0f, 0.0f)), mRotation(v3d(0.0f, 0.0f, 0.0f)), mScale(v3d(1.0f, 1.0f, 1.0f) ){}


void Transform::setRotation (v3d& rotation) { mRotation = q4d(rotation); }
void Transform::setPosition (v3d &position) { mPosition = position; }
void Transform::setScale    (v3d &scale)    { mScale = scale; }

const m4d& Transform::transformMatrix() const { return mTransformMatrix; }

void Transform::translate(float deltaX, float deltaY, float deltaZ){
    mPosition += v3d(deltaX, deltaY, deltaZ);
}

void Transform::rotateBy(float xRad, float yRad, float zRad){
    mRotation = q4d(xRad, 1.0f, 0.0f, 0.0f) * q4d(yRad, 0.0f, 1.0f, 0.0f) * q4d(zRad, 0.0f, 0.0f, 1.0f) * mRotation;
}

void Transform::rotateBy(float rad, v3d axis){
    mRotation = q4d(rad, axis) * mRotation;
}


#pragma mark helpers
void Transform::refreshTransformMatrix(){
    mTransformMatrix = m4d::translate(mPosition) * mRotation.matrix() * m4d::scale(mScale);
}


