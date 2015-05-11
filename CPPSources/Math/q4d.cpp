#include "q4d.h"
#include "m4d.h"

q4d::q4d(){}
q4d::q4d(const q4d& quaternion){
    for(int i = 0; i < 4; ++i){
        q[i] = quaternion.q[i];
    }
    normalize();
}
q4d::q4d(const v4d& vec){
    for(int i = 0; i < 4; ++i){
        q[i] = vec.v[i];
    }
    normalize();
}
q4d::q4d(const v3d& vec){
    q4d(v4d(vec));
}
q4d::q4d(const m4d& mat){
    float trace = mat.m[0][0] + mat.m[1][1] + mat.m[2][2];
    if(trace > 0.0f){
        float s = sqrt(trace + 1.0f);
        q[3] = s * 0.5f;
        
        float t = 0.5f / s;
        q[0] = (mat.m[2][1] - mat.m[1][2]) * t;
        q[1] = (mat.m[0][2] - mat.m[2][0]) * t;
        q[2] = (mat.m[1][0] - mat.m[0][1]) * t;
    }else{
        int i = 0;
        if(mat.m[1][1] > mat.m[0][0]) i = 1;
        if(mat.m[2][2] > mat.m[i][i]) i = 2;
        
        static const int NEXT[3] = {1, 2, 0};
        int j = NEXT[i];
        int k = NEXT[j];
        
        float s = sqrt((mat.m[i][j] - (mat.m[j][j] + mat.m[k][k])) + 1.0f);
        q[i] = s * 0.5f;
        float t;
        if(s != 0.0) t = 0.5f / s;
        else         t = s;
        
        q[3] = (mat.m[k][j] - mat.m[j][k]) * t;
        q[j] = (mat.m[j][i] - mat.m[i][j]) * t;
        q[k] = (mat.m[k][i] - mat.m[i][k]) * t;
    }
}
q4d::q4d(const float angle, float x, float y, float z) : q4d(angle, v3d(x, y, z)){}


q4d::q4d(const float angle, const v3d& vec){
    float rads      = PI / 180 * angle;
    float imCoef    = sinf(rads/2);
    float realCoef  = cosf(rads/2);
    for (int i = 0; i < 3; ++i)
        q[i] = vec.v[i] * imCoef;
    
    q[3] = realCoef;
    
    normalize();
}


float& q4d::operator[](int i){
    return i > 3 ? q[0] : q[1];
}
void q4d::operator=(const q4d& quaternion){
    q[0] = quaternion.q[0]; q[1] = quaternion.q[1]; q[2] = quaternion.q[2]; q[3] = quaternion.q[3];
}
q4d q4d::operator*(const float scale){
    q4d quaternion = q4d(q[0] * scale, q[1] * scale, q[2] * scale, q[3] * scale);
    return quaternion;
}
float q4d::magnitude(){
    return sqrtf( q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3] );
}
q4d q4d::conjugate(){
    return q4d(-q[0], -q[1], -q[2], q[3]);
}

m4d q4d::matrix(){
    m4d quatMatrix(1 - 2 * q[1] * q[1] - 2 * q[2] * q[2], 2 * q[0] * q[1] - 2 * q[3] * q[2], 2 * q[0] * q[2] + 2 * q[3] * q[1], 0,
                   2 * q[0] * q[1] + 2 * q[3] * q[2], 1 - 2 * q[0] * q[0] - 2 * q[2] * q[2], 2 * q[1] * q[2] - 2 * q[3] * q[0], 0,
                   2 * q[0] * q[2] - 2 * q[3] * q[1], 2 * q[1] * q[2] + 2 * q[3] * q[0], 1 - 2 * q[0] * q[0] - 2 * q[1] * q[1], 0,
                   0,                                   0,                                  0,                                  1);
    return quatMatrix;
}

void q4d::normalize(){
    float length = magnitude();
    q[0] /= length; q[1] /= length; q[2] /= length; q[3] /= length;
}

q4d q4d::unit(){
    q4d unitQuternion(*this);
    unitQuternion.normalize();
    return unitQuternion;
}

void q4d::print(const q4d& q4){
    std::cout << "[ " << q4.q[0] << ", " << q4.q[1] << ", " << q4.q[2] << ", " << q4.q[3] << " ]" << std::endl;
}

q4d q4d::lerp(const q4d &q1, const q4d &q2, float t){
    return q4d(q1 + (q2 - q1) * t);
}

q4d operator*(const q4d& q1, const q4d& q2){
    q4d quaternion;
    quaternion.q[0] = q1.q[0] * q2.q[3] + q1.q[1] * q2.q[2] - q1.q[2] * q2.q[1] + q1.q[3] * q2.q[0];
    quaternion.q[1] = -q1.q[0] * q2.q[2] + q1.q[1] * q2.q[3] + q1.q[2] * q2.q[0] + q1.q[3] * q2.q[1];
    quaternion.q[2] = q1.q[0] * q2.q[1] - q1.q[1] * q2.q[0] + q1.q[2] * q2.q[3] + q1.q[3] * q2.q[2];
    quaternion.q[3] = -q1.q[0] * q2.q[0] - q1.q[1] * q2.q[1] - q1.q[2] * q2.q[2] + q1.q[3] * q2.q[3];
    return quaternion;
}

q4d operator+(const q4d& q1, const q4d& q2){
    q4d quaternion(q1.q[0] + q2.q[0], q1.q[1] + q2.q[1], q1.q[2] + q2.q[2], q1.q[3] + q2.q[3]);
    return quaternion;
}

q4d operator-(const q4d& q1, const q4d& q2){
    q4d quaternion(q1.q[0] - q2.q[0], q1.q[1] - q2.q[1], q1.q[2] - q2.q[2], q1.q[3] - q2.q[3]);
    return quaternion;
}

