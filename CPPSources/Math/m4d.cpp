#include "m4d.h"
#include "v3d.h"

#pragma mark Constructors
m3d::m3d(){ m[0] = v3d(1.0f, 0.0f, 0.0f); m[1] = v3d(0.0f, 1.0f, 0.0f); m[2] = v3d(0.0f, 0.0f, 1.0f); }
m3d::m3d(const m4d& m4){ for(char i = 0; i < 3; ++i){ m[i] = v3d(m4.m[i].x, m4.m[i].y, m4.m[i].z); } }
m3d* m3d::operator=(const m4d &m4){ for(char i = 0; i < 3; ++i){ m[i] = v3d(m4.m[i].x, m4.m[i].y, m4.m[i].z); } return this; }
const float* m3d::pointer() const { return &(m[0].x);}

m4d::m4d():m4d(1.0f, .0f, .0f, .0f, .0f, 1.0f, .0f, .0f, .0f, .0f, 1.0f, .0f, .0f, .0f, .0f, 1.0f){}
m4d::m4d(const m3d& m3){ m[0] = v4d(m3.m[0], 1.0f); m[1] = v4d(m3.m[1], 1.0f); m[2] = v4d(m3.m[2], 1.0f); m[3] = v4d(.0f, .0f, .0f, 1.0f); }
m4d::m4d(const m4d& m4){ for(unsigned char i = 0; i < 4; i++){ m[i] = m4.m[i]; } }
m4d::m4d(const float arr[16]) : m4d(arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6], arr[7], arr[8],arr[9], arr[10], arr[11], arr[12], arr[13], arr[14], arr[15]){}
m4d::m4d(const float m00, const float m01, const float m02, const float m03,
         const float m10, const float m11, const float m12, const float m13,
         const float m20, const float m21, const float m22, const float m23,
         const float m30, const float m31, const float m32, const float m33){
    m[0] = v4d(m00, m01, m02, m03); m[1] = v4d(m10, m11, m12, m13); m[2] = v4d(m20, m21, m22, m23); m[3] = v4d(m30, m31, m32, m33);
}

void m4d::operator=(const m4d& m4){ for(unsigned char i = 0; i < 4; i++){ m[i] = m4.m[i]; } }

void m4d::print(const m4d& m){ for(int i = 0; i < 4; i++){ v4d::print(m.m[i]); } std::cout << std::endl; }

m4d operator*(const m4d& m1, const m4d& m2){
    m4d rMat4;
    m4d m2Transpose = m4d::transpose(m2);
    v4d temp;
    for(int row = 0; row < 4; row++){
        temp.x = v4d::dot(m1.m[row], m2Transpose.m[0]);
        temp.y = v4d::dot(m1.m[row], m2Transpose.m[1]);
        temp.z = v4d::dot(m1.m[row], m2Transpose.m[2]);
        temp.w = v4d::dot(m1.m[row], m2Transpose.m[3]);
        rMat4.m[row] = temp;
    }
    return rMat4;
}
m4d operator*(const float scalar, const m4d& m4){
    m4d rMat4;
    for(int i = 0; i < 4; i++){
        rMat4.m[i] = (m4.m[i] * scalar);
    }
    return rMat4;
}
m4d operator*(const m4d& m4, const float scalar){
    return operator*(scalar, m4);
}
m4d operator/(const m4d& m4, const float scalar){
    return operator*(m4, (1.0f / scalar));
}
m4d operator+(const m4d& m1, const m4d& m2){
    m4d rMat4;
    for(int i = 0; i < 4; i++){
        rMat4.m[i] = m1.m[i] + m2.m[i];
    }
    return rMat4;
}
m4d operator-(const m4d& m1, const m4d& m2){
    m4d rMat4;
    for(int i = 0; i < 4; i++){
        rMat4.m[i] = m1.m[i] - m2.m[i];
    }
    return rMat4;
}

//v4d operator*(const m4d& m, v4d& vec){
//    v4d rVec4;
//    rVec4.x = v4d::dot(m.m[0], vec);
//    rVec4.x = v4d::dot(m.m[1], vec);
//    rVec4.x = v4d::dot(m.m[2], vec);
//    rVec4.x = v4d::dot(m.m[3], vec);
//    return rVec4;
//}

v4d operator*(const m4d &m, const v4d &v){
    return v4d(m.m[0].x * v.x + m.m[0].y * v.y + m.m[0].z * v.z + m.m[0].w * v.w,
                  m.m[1].x * v.x + m.m[1].y * v.y + m.m[1].z * v.z + m.m[1].w * v.w,
                  m.m[2].x * v.x + m.m[2].y * v.y + m.m[2].z * v.z + m.m[2].w * v.w,
                  m.m[3].x * v.x + m.m[3].y * v.y + m.m[3].z * v.z + m.m[3].w * v.w);
}

v3d operator*(const v3d &v, const m4d &m){
    return v3d(m.m[0].x * v.x + m.m[0].y * v.y + m.m[0].z * v.z,
               m.m[1].x * v.x + m.m[1].y * v.y + m.m[1].z * v.z,
               m.m[2].x * v.x + m.m[2].y * v.y + m.m[2].z * v.z);
}

v3d operator*(const m4d &m, const v3d &v){
    return operator*(v, m);
}

v4d operator*(const v4d &v, const m4d &m){
    return operator*(m, v);
}

const float* m4d::pointer() const{
    return &(m[0].x);
}

// m4d operation definitions (all requared transformations)
float m4d::determinant(const m4d& m4){
    return
    m4.m[0].x * (m4.m[1].y * (m4.m[2].z * m4.m[3].w - m4.m[3].z * m4.m[2].w) - m4.m[1].z * (m4.m[2].y * m4.m[3].w - m4.m[3].y * m4.m[2].w) + m4.m[1].w * (m4.m[2].y * m4.m[3].z - m4.m[3].y * m4.m[2].z) )
    - m4.m[0].y * (m4.m[1].x * (m4.m[2].z * m4.m[3].w - m4.m[3].z * m4.m[2].w) - m4.m[1].z * (m4.m[2].x * m4.m[3].w - m4.m[3].x * m4.m[2].w) + m4.m[1].w * (m4.m[2].x * m4.m[3].z - m4.m[3].x * m4.m[2].z) )
    + m4.m[0].z * (m4.m[1].x * (m4.m[2].y * m4.m[3].w - m4.m[3].y * m4.m[2].w) - m4.m[1].y * (m4.m[2].x * m4.m[3].w - m4.m[3].x * m4.m[2].w) + m4.m[1].w * (m4.m[2].x * m4.m[3].y - m4.m[3].x * m4.m[2].y) )
    - m4.m[0].w * (m4.m[1].x * (m4.m[2].y * m4.m[3].z - m4.m[3].y * m4.m[2].z) - m4.m[1].y * (m4.m[2].x * m4.m[3].z - m4.m[3].x * m4.m[2].z) + m4.m[1].z * (m4.m[2].x * m4.m[3].y - m4.m[3].x * m4.m[2].y) );
}
m4d m4d::transpose(const m4d& m4){
    m4d rMat4;
    rMat4.m[0] = v4d(m4.m[0].x, m4.m[1].x, m4.m[2].x, m4.m[3].x);
    rMat4.m[1] = v4d(m4.m[0].y, m4.m[1].y, m4.m[2].y, m4.m[3].y);
    rMat4.m[2] = v4d(m4.m[0].z, m4.m[1].z, m4.m[2].z, m4.m[3].z);
    rMat4.m[3] = v4d(m4.m[0].w, m4.m[1].w, m4.m[2].w, m4.m[3].w);
    return rMat4;
}

m4d m4d::inverse(const m4d& m4){
    m4d rMat4;
    rMat4.m[0].x = m4.m[1].y * m4.m[2].z * m4.m[3].w + m4.m[1].z * m4.m[2].w * m4.m[3].y + m4.m[1].w * m4.m[2].y * m4.m[3].z - m4.m[1].y * m4.m[2].w * m4.m[3].z - m4.m[1].z * m4.m[2].y * m4.m[3].w - m4.m[1].w * m4.m[2].z * m4.m[3].y;
    rMat4.m[0].y = m4.m[0].y * m4.m[2].w * m4.m[3].z + m4.m[0].z * m4.m[2].y * m4.m[3].w + m4.m[0].w * m4.m[2].z * m4.m[3].y - m4.m[0].y * m4.m[2].z * m4.m[3].w - m4.m[0].z * m4.m[2].w * m4.m[3].y - m4.m[0].w * m4.m[2].y * m4.m[3].z;
    rMat4.m[0].z = m4.m[0].y * m4.m[1].z * m4.m[3].w + m4.m[0].z * m4.m[1].w * m4.m[3].y + m4.m[0].w * m4.m[1].y * m4.m[3].z - m4.m[0].y * m4.m[1].w * m4.m[3].z - m4.m[0].z * m4.m[1].y * m4.m[3].w - m4.m[0].w * m4.m[1].z * m4.m[3].y;
    rMat4.m[0].w = m4.m[0].y * m4.m[1].w * m4.m[2].z + m4.m[0].z * m4.m[1].y * m4.m[2].w + m4.m[0].w * m4.m[1].z * m4.m[2].y - m4.m[0].y * m4.m[1].z * m4.m[2].w - m4.m[0].z * m4.m[1].w * m4.m[2].y - m4.m[0].w * m4.m[1].y * m4.m[2].z;
    
    rMat4.m[1].x = m4.m[1].x * m4.m[2].w * m4.m[3].z + m4.m[1].z * m4.m[2].x * m4.m[3].w + m4.m[1].w * m4.m[2].z * m4.m[3].x - m4.m[1].x * m4.m[2].z * m4.m[3].w - m4.m[1].z * m4.m[2].w * m4.m[3].x - m4.m[1].w * m4.m[2].x * m4.m[3].z;
    rMat4.m[1].y = m4.m[0].x * m4.m[2].z * m4.m[3].w + m4.m[0].z * m4.m[2].w * m4.m[3].x + m4.m[0].w * m4.m[2].x * m4.m[3].z - m4.m[0].x * m4.m[2].w * m4.m[3].z - m4.m[0].z * m4.m[2].x * m4.m[3].w - m4.m[0].w * m4.m[2].z * m4.m[3].x;
    rMat4.m[1].z = m4.m[0].x * m4.m[1].w * m4.m[3].z + m4.m[0].z * m4.m[1].x * m4.m[3].w + m4.m[0].w * m4.m[1].z * m4.m[3].x - m4.m[0].x * m4.m[1].z * m4.m[3].w - m4.m[0].z * m4.m[1].w * m4.m[3].x - m4.m[0].w * m4.m[1].x * m4.m[3].z;
    rMat4.m[1].w = m4.m[0].x * m4.m[1].z * m4.m[2].w + m4.m[0].z * m4.m[1].w * m4.m[2].x + m4.m[0].w * m4.m[1].x * m4.m[2].z - m4.m[0].x * m4.m[1].w * m4.m[2].z - m4.m[0].z * m4.m[1].x * m4.m[2].w - m4.m[0].w * m4.m[1].z * m4.m[2].x;
    
    rMat4.m[2].x = m4.m[1].x * m4.m[2].y * m4.m[3].w + m4.m[1].y * m4.m[2].w * m4.m[3].x + m4.m[1].w * m4.m[2].x * m4.m[3].y - m4.m[1].x * m4.m[2].w * m4.m[3].y - m4.m[1].y * m4.m[2].x * m4.m[3].w - m4.m[1].w * m4.m[2].y * m4.m[3].x;
    rMat4.m[2].y = m4.m[0].x * m4.m[2].w * m4.m[3].y + m4.m[0].y * m4.m[2].x * m4.m[3].w + m4.m[0].w * m4.m[2].y * m4.m[3].x - m4.m[0].x * m4.m[2].y * m4.m[3].w - m4.m[0].y * m4.m[2].w * m4.m[3].x - m4.m[0].w * m4.m[2].x * m4.m[3].y;
    rMat4.m[2].z = m4.m[0].x * m4.m[1].y * m4.m[3].w + m4.m[0].y * m4.m[1].w * m4.m[3].x + m4.m[0].w * m4.m[1].x * m4.m[3].y - m4.m[0].x * m4.m[1].w * m4.m[3].y - m4.m[0].y * m4.m[1].x * m4.m[3].w - m4.m[0].w * m4.m[1].y * m4.m[3].x;
    rMat4.m[2].w = m4.m[0].x * m4.m[1].w * m4.m[2].y + m4.m[0].y * m4.m[1].x * m4.m[2].w + m4.m[0].w * m4.m[1].y * m4.m[2].x - m4.m[0].x * m4.m[1].y * m4.m[2].w - m4.m[0].y * m4.m[1].w * m4.m[2].x - m4.m[0].w * m4.m[1].x * m4.m[2].y;
    
    rMat4.m[3].x = m4.m[1].x * m4.m[2].z * m4.m[3].y + m4.m[1].y * m4.m[2].x * m4.m[3].z + m4.m[1].z * m4.m[2].y * m4.m[3].x - m4.m[1].x * m4.m[2].y * m4.m[3].z - m4.m[1].y * m4.m[2].z * m4.m[3].x - m4.m[1].z * m4.m[2].x * m4.m[3].y;
    rMat4.m[3].y = m4.m[0].x * m4.m[2].y * m4.m[3].z + m4.m[0].y * m4.m[2].z * m4.m[3].x + m4.m[0].z * m4.m[2].x * m4.m[3].y - m4.m[0].x * m4.m[2].z * m4.m[3].y - m4.m[0].y * m4.m[2].x * m4.m[3].z - m4.m[0].z * m4.m[2].y * m4.m[3].x;
    rMat4.m[3].z = m4.m[0].x * m4.m[1].z * m4.m[3].y + m4.m[0].y * m4.m[1].x * m4.m[3].z + m4.m[0].z * m4.m[1].y * m4.m[3].x - m4.m[0].x * m4.m[1].y * m4.m[3].z - m4.m[0].y * m4.m[1].z * m4.m[3].x - m4.m[0].z * m4.m[1].x * m4.m[3].y;
    rMat4.m[3].w = m4.m[0].x * m4.m[1].y * m4.m[2].z + m4.m[0].y * m4.m[1].z * m4.m[2].x + m4.m[0].z * m4.m[1].x * m4.m[2].y - m4.m[0].x * m4.m[1].z * m4.m[2].y - m4.m[0].y * m4.m[1].x * m4.m[2].z - m4.m[0].z * m4.m[1].y * m4.m[2].x;
    
    rMat4 = rMat4 / determinant(m4);
    return rMat4;
}
m4d m4d::inverseTranspose(const m4d& m4){
    return m4d::transpose(m4d::inverse(m4));
}
m4d m4d::lookAt(float eyeX, float eyeY, float eyeZ, float lookAtX, float lookAtY, float lookAtZ, float upX, float upY, float upZ){
    m4d rMat4;
    v3d eye(eyeX, eyeY, eyeZ);
    v3d lookAt(lookAtX, lookAtY, lookAtZ);
    v3d up(upX, upY, upZ);
    return m4d::lookAt(eye, lookAt, up);
}
m4d m4d::lookAt(v3d& eye, v3d& lookAt, v3d& up){
    m4d rMat4;
    v3d w = v3d::normalize(eye - lookAt);
    v3d u = v3d::normalize(v3d::cross(up, w));
    v3d v = v3d::normalize(v3d::cross(w, u));
    rMat4.m[0] = v4d(u, v3d::dot(u, -eye));
    rMat4.m[1] = v4d(v, v3d::dot(v, -eye));
    rMat4.m[2] = v4d(w, v3d::dot(w, -eye));
    return rMat4;
}

m4d m4d::ortho(float l, float r, float t, float b, float n, float f){
    float w = r - l;
    float h = t - b;
    float d = f - n;
    m4d rMat4( 2/w,   0,      0,        -(r + l)/w,
              0,    2/h,    0,        (t + b)/h,
              0,      0,   -2/d,      -(f + n)/d,
              0,      0,        0,         1);
    return rMat4;
}

m4d m4d::perspective(float fovy, float w, float h, float n, float f){
    float aspect = w / h;
    float A = - (f + n)/(f - n);
    float B = -(2 * f * n)/(f - n);
    float angleOfView =  PI / 180 * fovy;
    float d = 1.0f / tanf(angleOfView * 0.5f);
    
    m4d rMat4(d/aspect, 0, 0, 0,
              0,     d, 0, 0,
              0,     0, A, B,
              0,     0, -1,0);
    
    return rMat4;
}
m4d m4d::rotate(float angle, float x, float y, float z){
    v3d axis(x, y, z);
    return m4d::rotate(angle, axis);
}
m4d m4d::rotate(v3d &angles){
    return m4d::rotate(angles.x, 1.0f, .0f, .0f) * m4d::rotate(angles.y, .0f, 1.0f, .0f) * m4d::rotate(angles.z, .0f, .0f, 1.0f);
}

m4d m4d::rotate(float angle, v3d& axis){
    float degree = PI / 180 * angle;
    v3d nAxis = v3d::normalize(axis);
    float x = nAxis.x;
    float y = nAxis.y;
    float z = nAxis.z;
    float c = cos(degree);
    float s = sin(degree);
    
    m4d rMat4(c + (1 - c)* x * x, (1 - c) * x * y - s * z, (1 - c) * x * z + s * y, 0,
              (1 - c) * x * y + s * z, c + (1 - c) * y * y, (1 - c) * y * z - s * x, 0,
              (1- c) * x * z - s * y, (1 - c) * y * z + s * x, c + (1 - c) * z * z, 0,
              0, 0, 0, 1);
    
    return rMat4;
}
m4d m4d::scale(float x, float y, float z){
    v3d scale(x, y, z);
    return m4d::scale(scale);
}
m4d m4d::scale(v3d& scale){
    m4d rMat4(scale.x, 0, 0, 0,
              0, scale.y, 0, 0,
              0, 0, scale.z, 0,
              0,	  0,   0,  1);
    return rMat4;
}
m4d m4d::translate(float x, float y, float z){
    v3d translate(x, y, z);
    return m4d::translate(translate);
}
m4d m4d::translate(v3d& translate){
    m4d rMat4(1, 0, 0, translate.x,
              0, 1, 0, translate.y,
              0, 0, 1, translate.z,
              0, 0, 0,      1);
    
    return rMat4;
}
