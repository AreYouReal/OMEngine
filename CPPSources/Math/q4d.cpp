#include "q4d.h"


q4d::q4d(){}
q4d::q4d(const q4d& quaternion){
    x = quaternion.x; y = quaternion.y; z = quaternion.z; w = quaternion.w;
    normalize();
}
q4d::q4d(const v4d& vec){
    x = vec.x; y = vec.y; z = vec.z; w = vec.w;
    normalize();
}
//q4d::q4d(const v3d& vec){
//    q4d(v4d(vec));
//}
q4d::q4d(const m4d& mat){
//    float trace = mat.m[0].x + mat.m[1].y + mat.m[2].z;
//    if(trace > 0.0f){
//        float s = sqrt(trace + 1.0f);
//        w = s * 0.5f;
//        
//        float t = 0.5f / s;
//        x = (mat.m[2].y - mat.m[1].z) * t;
//        y = (mat.m[0].z - mat.m[2].x) * t;
//        z = (mat.m[1].x - mat.m[0].y) * t;
//    }else{
//        int i = 0;
//        if(mat.m[1].y > mat.m[0].x) i = 1;
//        switch (i) {
//            case 0: if(mat.m[2].z > mat.m[0].x) i = 2; break;
//            case 1: if(mat.m[2].z > mat.m[1].y) i = 2; break;
//            case 2: if(mat.m[2].z > mat.m[2].z) i = 2; break;
//            case 3: if(mat.m[2].z > mat.m[3].w) i = 2; break;
//            default: break;
//        }
//
//        
//        static const int NEXT[3] = {1, 2, 0};
//        int j = NEXT[i];
//        int k = NEXT[j];
//        
//        float a = 0.0f;
//        switch (j) {
//            case 0: a = mat.m[i].x - mat.m[j].x; break;
//            case 1: a = mat.m[i].y - mat.m[j].y; break;
//            case 2: a = mat.m[i].z - mat.m[j].z; break;
//            case 3: a = mat.m[i].w - mat.m[j].w; break;
//            default: break;
//        }
//        float b = 0.0f;
//        switch (k) {
//            case 0: b = mat.m[k].x; break;
//            case 1: b = mat.m[k].y; break;
//            case 2: b = mat.m[k].z; break;
//            case 3: b = mat.m[k].w; break;
//            default: break;
//        }
//        
//        float s = sqrt((a + b) + 1.0f);
//        
////        q[i] = s * 0.5f;
//        switch (i) {
//            case 0: x = s * 0.5f; break;
//            case 1: y = s * 0.5f; break;
//            case 2: z = s * 0.5f; break;
//            case 3: w = s * 0.5f; break;
//            default: break;
//        }
//        float t;
//        if(s != 0.0) t = 0.5f / s;
//        else         t = s;
//        w = (mat.m[k][j] - mat.m[j][k]) * t;
//        switch (j) {
//            case 0: x = (mat.m[j][i] - mat.m[i][j]) * t; break;
//            case 1: y = (mat.m[j][i] - mat.m[i][j]) * t; break;
//            case 2: z = (mat.m[j][i] - mat.m[i][j]) * t; break;
//            case 3: w = (mat.m[j][i] - mat.m[i][j]) * t; break;
//            default: break;
//        }
//        switch (k) {
//            case 0: x = (mat.m[k][i] - mat.m[i][k]) * t; break;
//            case 1: y = (mat.m[k][i] - mat.m[i][k]) * t; break;
//            case 2: z = (mat.m[k][i] - mat.m[i][k]) * t; break;
//            case 3: w = (mat.m[k][i] - mat.m[i][k]) * t; break;
//            default: break;
//        }
////        q[j] = (mat.m[j][i] - mat.m[i][j]) * t;
////        q[k] = (mat.m[k][i] - mat.m[i][k]) * t;
//    }
}
q4d::q4d(const float angle, float x, float y, float z) : q4d(angle, v3d(x, y, z)){}


q4d::q4d(const float angle, const v3d& vec){
    float rads      = PI / 180 * angle;
    float imCoef    = sin(rads * 0.5f);
    float realCoef  = cos(rads * 0.5f);
    v3d nAxis = v3d::normalize(vec);
    x = nAxis.x * imCoef;
    y = nAxis.y * imCoef;
    z = nAxis.z * imCoef;
    w = realCoef;
    normalize();
}


float q4d::operator[](int i){
    switch (i) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        case 3: return w;
        default: return x;
    }
}
void q4d::operator=(const q4d& quaternion){
    x = quaternion.x; y = quaternion.y; z = quaternion.z; w = quaternion.w;
}
q4d q4d::operator*(const float scale){
    q4d quaternion = q4d(x * scale, y * scale, z * scale, w * scale);
    return quaternion;
}

void q4d::operator*=(const q4d& quat){
    *this = (*this * quat);
}

void q4d::calculateWFromXYZ(){
    float l = 1.0f - (x * x) - (y * y) - (z * z);
    w = (l < 0.0f) ? 0.0f : -sqrt(l);
}

float q4d::magnitude(){
    return sqrtf( x * x + y * y + z * z + w * w );
}
q4d q4d::conjugate(){
    return q4d(-x, -y, -z, w);
}

m4d q4d::matrix(){
    m4d quatMatrix(1 - 2 * y * y - 2 * z * z, 2 * x * y - 2 * w * z, 2 * x * z + 2 * w * y, 0,
                   2 * x * y + 2 * w * z, 1 - 2 * x * x - 2 * z * z, 2 * y * z - 2 * w * x, 0,
                   2 * x * z - 2 * w * y, 2 * y * z + 2 * w * x, 1 - 2 * x * x - 2 * y * y, 0,
                   0,                                   0,                                  0,                                  1);
    return quatMatrix;
}

void q4d::normalize(){
    float length = magnitude();
    x /= length; y /= length; z /= length; w /= length;
}

q4d q4d::unit(){
    q4d unitQuternion(*this);
    unitQuternion.normalize();
    return unitQuternion;
}

void q4d::print(const q4d& q4){
    std::cout << "[ " << q4.x << ", " << q4.y << ", " << q4.z << ", " << q4.w << " ]" << std::endl;
}

q4d q4d::lerp(const q4d &q1, const q4d &q2, float t){
    return q4d(q1 + (q2 - q1) * t);
}

q4d operator*(const q4d& q1, const q4d& q2){
    q4d quaternion;
    quaternion.x = q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x;
    quaternion.y = -q1.x * q2.z + q1.y * q2.w + q1.z * q2.x + q1.w * q2.y;
    quaternion.z = q1.x * q2.y - q1.y * q2.x + q1.z * q2.w + q1.w * q2.z;
    quaternion.w = -q1.x * q2.x - q1.y * q2.y - q1.z * q2.z + q1.w * q2.w;
    return quaternion;
}

q4d operator+(const q4d& q1, const q4d& q2){
    q4d quaternion(q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w);
    return quaternion;
}

q4d operator-(const q4d& q1, const q4d& q2){
    q4d quaternion(q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w);
    return quaternion;
}

