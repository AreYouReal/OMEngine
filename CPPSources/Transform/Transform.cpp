#include "Transform.hpp"


Transform::Transform() : mPosition(v3d(0.0f, 0.0f, 0.0f)), mRotation(v3d(0.0f, 0.0f, 0.0f)), mScale(v3d(1.0f, 1.0f, 1.0f) ){}

Transform::Transform(v3d& position): Transform() {
    mPosition = position;
}

const m4d& Transform::transformMatrix() const { return mTransformMatrix; }

void Transform::translate(v3d tVector){
    translate(tVector.x, tVector.y, tVector.z);
}

inline void Transform::translate(float deltaX, float deltaY, float deltaZ){
    mPosition += v3d(deltaX, deltaY, deltaZ);
}

void Transform::rotate(float xRad, float yRad, float zRad){
    mRotation = q4d(xRad, 1.0f, 0.0f, 0.0f) * q4d(yRad, 0.0f, 1.0f, 0.0f) * q4d(zRad, 0.0f, 0.0f, 1.0f) * mRotation;
//    mFront = mFront * mRotation.matrix();
//    mUp = mUp * mRotation.matrix();
}

void Transform::rotate(float rad, v3d axis){
    mRotation = q4d(rad, axis) * mRotation;
}

void Transform::scale(float xFactor, float yFactor, float zFactor){
    mScale.x += xFactor; mScale.y += yFactor; mScale.z = zFactor;
}

void Transform::scale(v3d &scaleVec){
    scale(scaleVec.x, scaleVec.y, scaleVec.z);
}


#pragma mark helpers
void Transform::refreshTransformMatrix(){
    mTransformMatrix = m4d::translate(mPosition) * mRotation.matrix() * m4d::scale(mScale);
}


