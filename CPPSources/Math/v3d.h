#pragma once

#include <cmath>

// TODO: Remove from release
#include <iostream>

#define PI atan(1.0f)*4.0f


// Forward declaration
struct v3d;
struct m4d;

struct v4d{
public:
    v4d();
    v4d(float x, float y, float z, float w = 0.0f);
    v4d(const v3d& vec3);
    
    float& operator[](int i);
    void operator=(const v4d& vec);
    
    float v[4]{0.0, 0.0, 0.0, 1.0};
    
    static void print(const v4d& vec4);
};



v4d operator*(const m4d& m, v4d& vec);

struct v3d{
public:
    v3d();
    v3d(float a);
    v3d(float x, float y, float z);
    v3d(const v4d& vec);
    
    float& operator[](int i);
    void operator=(const v3d& vec);
    
    float* pointer();
    
    float v[3]{0.0, 0.0, 0.0};

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

