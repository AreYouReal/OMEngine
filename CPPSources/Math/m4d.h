#pragma once

#include "v3d.h"

struct m4d{
public:
    m4d();
    m4d(const m4d& m4);
    m4d(const float arr[16]);
    m4d(const float m00, const float m01, const float m02, const float m03,
        const float m10, const float m11, const float m12, const float m13,
        const float m20, const float m21, const float m22, const float m23,
        const float m30, const float m31, const float m32, const float m33);
    
    void operator=(const m4d& m4);
    float* pointer();
    
    
    v4d m[4];
    
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
    static m4d rotate(v3d& angles);
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
v4d operator*(const m4d& m, const v4d &v);
v4d operator*(const v4d& v, const m4d &m);
v3d operator*(const v3d& v, const m4d &m);


