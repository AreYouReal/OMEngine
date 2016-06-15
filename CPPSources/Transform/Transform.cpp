#include "Transform.hpp"

#include "OMUtils.h"

v3d Transform::baseFront{0, 0, 1};
v3d Transform::baseUp{0, 1, 0};

Transform::Transform() : mPosition(0.0f, 0.0f, 0.0f), mRotation(0.0f, 0.0f, 0.0f, 0.0f), mScale(1.0f, 1.0f, 1.0f) {
    mFront = v3d(0, 0, 1);
    mRight = v3d(1, 0, 0);
    refreshTransformMatrix();
}

Transform::Transform(v3d& position): Transform() {
    mPosition = position;
    mUp = v3d(0, 1, 0);
    mFront = v3d(0, 0, 1);
    mRight = v3d(1, 0, 0);
    refreshTransformMatrix();
}

Transform::Transform(v3d&& position): Transform() {
    mPosition = std::move(position);
    mUp = v3d(0, 1, 0);
    mFront = v3d(0, 0, 1);
    mRight = v3d(1, 0, 0);
    refreshTransformMatrix();
}

Transform::~Transform(){
}

const m4d& Transform::transformMatrix() const {
    return mTransformMatrix; }

void Transform::translate(v3d tVector){
    translate(tVector.x, tVector.y, tVector.z);
    refreshTransformMatrix();
}

void Transform::translate(float deltaX, float deltaY, float deltaZ){
    mPosition += v3d(deltaX, deltaY, deltaZ);
    refreshTransformMatrix();
}

void Transform::rotate(float xRad, float yRad, float zRad){
    mRotation = q4d(xRad, v3d(1.0f, 0.0f, 0.0f)) * q4d(yRad, v3d(0.0f, 1.0f, 0.0f)) * q4d(zRad, v3d(0.0f, 0.0f, 1.0f));
    rotate(mRotation);
}

void Transform::rotate(float deg, v3d axis){
    mRotation = q4d(deg, axis);
    rotate(mRotation);
}

void Transform::rotate(q4d rotation){
    mRotation = rotation;
    q4d::print(rotation);
    mFront = baseFront * mRotation.matrix();
    mUp = baseUp * mRotation.matrix();
    mRight = v3d::cross(mUp, mFront);
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


