#pragma once

#include "v3d.h"
#include "m4d.h"

struct q4d{
public:
    q4d();
    q4d(const q4d& quaternion);
    q4d(const v4d& vec);
    q4d(const v3d& vec);
    q4d(const m4d& mat);
    q4d(float x, float y, float z, float w);
    q4d(const float angle, const v3d& axis);
    
    float operator[](int i);
    void operator=(const q4d& quat);
    q4d operator*(const float scale);
    void operator*=(const q4d& quat);
    
    
    // Quaternion behaviour
    void calculateWFromXYZ();
    
    float magnitude();
    q4d conjugate();
    m4d matrix();
    void normalize();
    q4d unit();
    
    float x, y, z, w = 1.0f;

    
    static void print(const q4d& quat);
    static q4d lerp(const q4d& q1, const q4d& q2, float t);
    static q4d slerp(const q4d& q1, const q4d& q2, float t);

};

q4d operator*(const q4d& q1, const q4d& q2);
q4d operator*(const q4d& q1, const float scale);
q4d operator+(const q4d& q1, const q4d& q2);
q4d operator-(const q4d& q1, const q4d& q2);;