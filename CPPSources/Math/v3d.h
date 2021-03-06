#pragma once

#include <cmath>

// TODO: Remove from release
#include <iostream>

const float PI = atan(1.0f)*4.0f;


// Forward declaration
struct v3d;
struct m4d;

struct v4d{
public:
    v4d();
    v4d(float x, float y, float z, float w = 0.0f);
    v4d(const v4d& v);
    v4d(const v3d& vec3);
    v4d(const v3d& vec3, const float w);
    
    const float& operator[](int i);
    v4d& operator=(const v4d& vec);
    
    float x, y, z, w;
    
    static float dot(const v4d& v1, const v4d& v2);
    
    static void print(const v4d& vec4);
};

struct v3d{
    v3d();
    v3d(float a);
    v3d(float x, float y, float z);
    v3d(const v4d& vec);
    
    void operator+=(const v3d& vec);
    float operator[](int i);
    v3d& operator=(const v3d& vec);
    
    float* pointer();
    
    float x, y, z;

    static float dot(const v3d& v1, const v3d& v2);
    static v3d cross(const v3d& v1, const v3d& v2);
    static float length(const v3d& vec3);
    static v3d normalize(const v3d& vec);
    
    static void print(const v3d& vec3);
};


v3d operator*(const float scalar, const v3d& vec3);
v3d operator*(const v3d& vec3, const float scalar);
v3d operator+(const v3d& v1, const v3d& v2);
v3d operator-(const v3d& v1, const v3d& v2);
v3d operator-(const v3d& vec);
v3d operator/(v3d& vec3, const float scalar);

v4d operator+(const v4d& v1, const v4d& v2);
v4d operator-(const v4d& v1, const v4d& v2);
v4d operator*(const v4d& vec4, const float scalar);
