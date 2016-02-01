#include "Transform.hpp"

#include "OMUtils.h"

Transform::Transform() : mPosition(0.0f, 0.0f, 0.0f), mRotation(0.0f, 0.0f, 0.0f, 0.0f), mScale(1.0f, 1.0f, 1.0f) {
}

Transform::Transform(v3d& position): Transform() {
    mPosition = position;
    refreshTransformMatrix();
}

Transform::Transform(v3d&& position): Transform() {
    mPosition = std::move(position);
    refreshTransformMatrix();
}

Transform::~Transform(){
}

const m4d& Transform::transformMatrix() const { return mTransformMatrix; }

void Transform::translate(v3d tVector){
    translate(tVector.x, tVector.y, tVector.z);
    refreshTransformMatrix();
}

inline void Transform::translate(float deltaX, float deltaY, float deltaZ){
    mPosition += v3d(deltaX, deltaY, deltaZ);
    refreshTransformMatrix();
}

void Transform::rotate(float xRad, float yRad, float zRad){
    mRotation = q4d(xRad, 1.0f, 0.0f, 0.0f) * q4d(yRad, 0.0f, 1.0f, 0.0f) * q4d(zRad, 0.0f, 0.0f, 1.0f);
    
    mFront = mFront * mRotation.matrix();
    mUp = mUp * mRotation.matrix();
    mRight = v3d::cross(mUp, mFront);
    refreshTransformMatrix();
}

void Transform::rotate(float deg, v3d axis){
    mRotation = q4d(deg, axis);
    m4d roMatrix = mRotation.matrix();
    mFront = (mFront * roMatrix).normalize();
    mUp = (mUp * roMatrix).normalize();
    mRight = v3d::cross(mUp, mFront).normalize();
    refreshTransformMatrix();
}

void Transform::scale(float xFactor, float yFactor, float zFactor){
    mScale.x += xFactor; mScale.y += yFactor; mScale.z = zFactor;
    refreshTransformMatrix();
}

void Transform::scale(v3d &scaleVec){
    scale(scaleVec.x, scaleVec.y, scaleVec.z);
    refreshTransformMatrix();
}

void Transform::moveForward(float velocity){
    mPosition += v3d::normalize(mFront) * velocity;
    refreshTransformMatrix();
}

void Transform::moveRight(float velocity){
    mPosition += mRight * velocity;
        refreshTransformMatrix();
}

#pragma mark helpers
void Transform::refreshTransformMatrix(){
    mTransformMatrix = m4d::translate(mPosition) * mRotation.matrix() * m4d::scale(mScale);
}


