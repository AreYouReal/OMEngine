#pragma once

#include "v3d.h"

struct m4d{
public:
    m4d();
    m4d(const m4d& m4);
    m4d(const float arr[16]);
    m4d(float m11, float m12, float m13, float m14,
        float m21, float m22, float m23, float m24,
        float m31, float m32, float m33, float m34,
        float m41, float m42, float m43, float m44);
    
    void operator=(const m4d& m4);
    float* pointer();
    
    
    float m[4][4];
    
    static float determinant(const m4d& m4);
    static m4d transpose(const m4d& m4);
    static m4d inverse(const m4d& m4);
    static m4d inverseTranspose(const m4d& m4);
    static m4d lookAt(float eyeX, float eyeY, float eyeZ, float lookAtX, float lookAtY, float lookAtZ, float upX, float upY, float upZ);
    static m4d lookAt(v3d& eye, v3d& lookAt, v3d& up);
    static m4d ortho(float l, float r, float t, float b, float n, float f);
    static m4d perspective(float fovy, float w, float h, float n, float f);
    static m4d rotate(float angle, float x, float y, float z);
    static m4d rotate(float angle, v3d& axis);
    static m4d scale(float x, float y, float z);
    static m4d scale(v3d& scale);
    static m4d translate(float x, float y, float z);
    static m4d translate(v3d& translate);
    
    static void print(const m4d& m);
    
};


m4d operator*(const m4d& m1, const m4d& m2);
m4d operator*(const float scalar, const m4d& m4);
m4d operator*(const m4d& m4, const float scalar);
m4d operator/(const m4d& m4, const float scalar);
m4d operator+(const m4d& m1, const m4d& m2);
m4d operator-(const m4d& m1, const m4d& m2);

