#include "v3d.h"


#pragma mark 3D
v3d::v3d(){}
v3d::v3d(float a):x(a), y(a),z(a){}
v3d::v3d(float x, float y, float z): x(x), y(y), z(z){}
v3d::v3d(const v4d& vec){
    x = vec.x / vec.w; y = vec.y / vec.w; z = vec.z / vec.w;
}
float v3d::operator[](int i){
    switch (i) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        default: return x;
    }
};

v3d& v3d::operator=(const v3d& vec3){
    x = vec3.x;	y = vec3.y;	z = vec3.z;
    return *this;
}

void v3d::operator+=(const v3d &vec){
    x += vec.x; y += vec.y; z += vec.z;
}

// Global v3d operator definitions
void v3d::print(const v3d& vec3){
    std::cout << "[ " << vec3.x << ", " << vec3.y << ", " << vec3.z << " ]" << std::endl;
}
v3d operator+(const v3d& v1, const v3d& v2){
    v3d rVec3;
    rVec3.x = v1.x + v2.x;	rVec3.y = v1.y + v2.y;	rVec3.z = v1.z + v2.z;
    return rVec3;
}
v3d operator-(const v3d& v2, const v3d& v1){
    v3d rVec3;
    rVec3.x = v2.x - v1.x;	rVec3.y = v2.y - v1.y;	rVec3.z = v2.z - v1.z;
    return rVec3;
}
v3d operator-(const v3d& vec){
    v3d rVec3;
    rVec3.x = -vec.x; rVec3.y = -vec.y; rVec3.z = -vec.z;
    return rVec3;
}
v3d operator*(const float scalar, const v3d& vec3){
    v3d rVec3;
    rVec3.x = scalar * vec3.x;	rVec3.y = scalar * vec3.y;	rVec3.z = scalar * vec3.z;
    return rVec3;
}
v3d operator*(const v3d& vec3, const float scalar){
    return operator*(scalar, vec3);
}
v3d operator/(const v3d& vec3, const float scalar){
    v3d rVec3;
    rVec3.x = vec3.x / scalar;	rVec3.y = vec3.y / scalar;	rVec3.z = vec3.z / scalar;
    return rVec3;
}

float v3d::dot(const v3d& v1, const v3d& v2){
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
};
v3d v3d::cross(const v3d& v1, const v3d& v2){
    v3d rVec3;
    rVec3.x = v1.y * v2.z - v1.z * v2.y;
    rVec3.y = v1.z * v2.x - v1.x * v2.z;
    rVec3.z = v1.x * v2.y - v1.y * v2.x;
    return rVec3;
}
float v3d::length(const v3d& vec3){
    return sqrtf(dot(vec3, vec3));
}
v3d v3d::normalize(const v3d& vec3){
    v3d rVec3;
    float length = v3d::length(vec3);
    if(length){
        float m = 1.0f / length;
        rVec3.x = vec3.x * m;
        rVec3.y = vec3.y * m;
        rVec3.z = vec3.z * m;
    }
    return rVec3;
}

float* v3d::pointer(){
    return &x;
}

#pragma mark 4D
v4d::v4d(){}
v4d::v4d(float x, float y, float z, float w):x(x), y(y), z(z), w(w){}
v4d::v4d(const v3d& vec){ x = vec.x; y = vec.y; z = vec.z; w = 1.0f; }

float v4d::operator[](int i){
    switch (i) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        case 3: return w;
        default: return x;
    }
}
v4d& v4d::operator=(const v4d& vec){
    x = vec.x; y = vec.y; z = vec.z; w = vec.w;
    return *this;
}

void v4d::print(const v4d& vec4){
    std::cout << "[ " << vec4.x << ", " << vec4.y << ", " << vec4.z << ", " << vec4.w << " ]" << std::endl;
}




