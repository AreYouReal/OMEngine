#include "v3d.h"


#pragma mark 3D
v3d::v3d(){}
v3d::v3d(float a){
    v[0] = v[1] = v[2] = a;
}
v3d::v3d(float x, float y, float z){
    v[0] = x; v[1] = y; v[2] = z;
}
v3d::v3d(const v4d& vec){
    v[0] = vec.v[0] / vec.v[3]; v[1] = vec.v[1] / vec.v[3]; v[2] = vec.v[2] / vec.v[3];
}
float& v3d::operator[](int i){
    return i > 2 ? v[0] : v[i];
};

void v3d::operator=(const v3d& vec3){
    v[0] = vec3.v[0];	v[1] = vec3.v[1];	v[2] = vec3.v[2];
}

// Global v3d operator definitions
void v3d::print(const v3d& vec3){
    std::cout << "[ " << vec3.v[0] << ", " << vec3.v[1] << ", " << vec3.v[2] << " ]" << std::endl;
}
v3d operator+(const v3d& v1, const v3d& v2){
    v3d rVec3;
    rVec3.v[0] = v1.v[0] + v2.v[0];	rVec3.v[1] = v1.v[1] + v2.v[1];	rVec3.v[2] = v1.v[2] + v2.v[2];
    return rVec3;
}
v3d operator-(const v3d& v2, const v3d& v1){
    v3d rVec3;
    rVec3.v[0] = v2.v[0] - v1.v[0];	rVec3.v[1] = v2.v[1] - v1.v[1];	rVec3.v[2] = v2.v[2] - v1.v[2];
    return rVec3;
}
v3d operator-(const v3d& vec){
    v3d rVec3;
    rVec3.v[0] = -vec.v[0]; rVec3.v[1] = -vec.v[1]; rVec3.v[2] = -vec.v[2];
    return rVec3;
}
v3d operator*(const float scalar, const v3d& vec3){
    v3d rVec3;
    rVec3.v[0] = scalar * vec3.v[0];	rVec3.v[1] = scalar * vec3.v[1];	rVec3.v[2] = scalar * vec3.v[2];
    return rVec3;
}
v3d operator*(const v3d& vec3, const float scalar){
    return operator*(scalar, vec3);
}
v3d operator/(const v3d& vec3, const float scalar){
    v3d rVec3;
    rVec3.v[0] = vec3.v[0] / scalar;	rVec3.v[1] = vec3.v[1] / scalar;	rVec3.v[2] = vec3.v[2] / scalar;
    return rVec3;
}

float v3d::dot(const v3d& v1, const v3d& v2){
    return (v1.v[0] * v2.v[0]) + (v1.v[1] * v2.v[1]) + (v1.v[2] * v2.v[2]);
};
v3d v3d::cross(const v3d& v1, const v3d& v2){
    v3d rVec3;
    rVec3.v[0] = v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1];
    rVec3.v[1] = v1.v[2] * v2.v[0] - v1.v[0] * v2.v[2];
    rVec3.v[2] = v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0];
    return rVec3;
}
float v3d::length(const v3d& vec3){
    return sqrtf(dot(vec3, vec3));
}
v3d v3d::normalize(const v3d& vec3){
    v3d rVec3;
    float length = v3d::length(vec3);
    rVec3.v[0] = vec3.v[0] / length;
    rVec3.v[1] = vec3.v[1] / length;
    rVec3.v[2] = vec3.v[2] / length;
    return rVec3;
}

float* v3d::pointer(){
    return v;
}

#pragma mark 4D
v4d::v4d(){}
v4d::v4d(float x, float y, float z, float w){
    v[0] = x; v[1] = y; v[2] = z; v[3] = w;
}
v4d::v4d(const v3d& vec){
    for(int i = 0; i < 3; ++i)
        v[i] = vec.v[i];
    v[3] = 0.0f;
}

float& v4d::operator[](int i){
    return i > 3 ? v[0] : v[i];
}
void v4d::operator=(const v4d& vec){
    v[0] = vec.v[0]; v[1] = vec.v[1]; v[2] = vec.v[2]; v[3] = vec.v[3];
}

void v4d::print(const v4d& vec4){
    std::cout << "[ " << vec4.v[0] << ", " << vec4.v[1] << ", " << vec4.v[2] << ", " << vec4.v[3] << " ]" << std::endl;
}




