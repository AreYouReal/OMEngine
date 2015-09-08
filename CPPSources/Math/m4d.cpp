#include "m4d.h"
#include "v3d.h"

m4d::m4d(){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(i == j)  m[i][j] = 1.0f;
            else        m[i][j] = 0.0f;
        }
    }
}
m4d::m4d(const m4d& m4){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            m[i][j] = m4.m[i][j];
        }
    }
}
m4d::m4d(const float arr[16]){
    int offset = 0;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            m[i][j] = arr[offset++];
        }
    }
}

m4d::m4d(float m00, float m01, float m02, float m03,
         float m10, float m11, float m12, float m13,
         float m20, float m21, float m22, float m23,
         float m30, float m31, float m32, float m33){
    m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
    m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
    m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
    m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
}

void m4d::operator=(const m4d& m4){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            m[i][j] = m4.m[i][j];
        }
    }
}

void m4d::print(const m4d& m){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            std::cout << m.m[i][j] << "  ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
m4d operator*(const m4d& m1, const m4d& m2){
    m4d rMat4;
    for(int row = 0; row < 4; row++){
        for(int col = 0; col < 4; col++){
            float temp = 0;
            for(int i = 0; i < 4; i++){
                temp += m1.m[row][i] * m2.m[i][col];
            }
            rMat4.m[row][col] = temp;
        }
    }
    return rMat4;
}
m4d operator*(const float scalar, const m4d& m4){
    m4d rMat4;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            rMat4.m[i][j] = scalar * m4.m[i][j];
        }
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
        for(int j = 0; j < 4; j++){
            rMat4.m[i][j] = m1.m[i][j] + m2.m[i][j];
        }
    }
    return rMat4;
}
m4d operator-(const m4d& m1, const m4d& m2){
    m4d rMat4;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            rMat4.m[i][j] = m1.m[i][j] - m2.m[i][j];
        }
    }
    return rMat4;
}

v4d operator*(const m4d& m, v4d& vec){
    v4d rVec4;
    for(int row = 0; row < 4; row++){
        float result = 0;
        for(int col = 0; col < 4; col++){
            result += m.m[row][col] * vec[col];
        }
        switch (row) {
            case 0: rVec4.x = result; break;
            case 1: rVec4.y = result; break;
            case 2: rVec4.z = result; break;
            case 3: rVec4.w = result; break;
            default: break;
        }
    }
    return rVec4;
}

float* m4d::pointer(){
    return &(m[0][0]);
}

// m4d operation definitions (all requared transformations)
float m4d::determinant(const m4d& m4){
    return
    m4.m[0][0] * (m4.m[1][1] * (m4.m[2][2] * m4.m[3][3] - m4.m[3][2] * m4.m[2][3]) - m4.m[1][2] * (m4.m[2][1] * m4.m[3][3] - m4.m[3][1] * m4.m[2][3]) + m4.m[1][3] * (m4.m[2][1] * m4.m[3][2] - m4.m[3][1] * m4.m[2][2]) )
    - m4.m[0][1] * (m4.m[1][0] * (m4.m[2][2] * m4.m[3][3] - m4.m[3][2] * m4.m[2][3]) - m4.m[1][2] * (m4.m[2][0] * m4.m[3][3] - m4.m[3][0] * m4.m[2][3]) + m4.m[1][3] * (m4.m[2][0] * m4.m[3][2] - m4.m[3][0] * m4.m[2][2]) )
    + m4.m[0][2] * (m4.m[1][0] * (m4.m[2][1] * m4.m[3][3] - m4.m[3][1] * m4.m[2][3]) - m4.m[1][1] * (m4.m[2][0] * m4.m[3][3] - m4.m[3][0] * m4.m[2][3]) + m4.m[1][3] * (m4.m[2][0] * m4.m[3][1] - m4.m[3][0] * m4.m[2][1]) )
    - m4.m[0][3] * (m4.m[1][0] * (m4.m[2][1] * m4.m[3][2] - m4.m[3][1] * m4.m[2][2]) - m4.m[1][1] * (m4.m[2][0] * m4.m[3][2] - m4.m[3][0] * m4.m[2][2]) + m4.m[1][2] * (m4.m[2][0] * m4.m[3][1] - m4.m[3][0] * m4.m[2][1]) );
}
m4d m4d::transpose(const m4d& m4){
    m4d rMat4;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            rMat4.m[i][j] = m4.m[j][i];
        }
    }
    return rMat4;
}

m4d m4d::inverse(const m4d& m4){
    m4d rMat4;
    rMat4.m[0][0] = m4.m[1][1] * m4.m[2][2] * m4.m[3][3] + m4.m[1][2] * m4.m[2][3] * m4.m[3][1] + m4.m[1][3] * m4.m[2][1] * m4.m[3][2] - m4.m[1][1] * m4.m[2][3] * m4.m[3][2] - m4.m[1][2] * m4.m[2][1] * m4.m[3][3] - m4.m[1][3] * m4.m[2][2] * m4.m[3][1];
    rMat4.m[0][1] = m4.m[0][1] * m4.m[2][3] * m4.m[3][2] + m4.m[0][2] * m4.m[2][1] * m4.m[3][3] + m4.m[0][3] * m4.m[2][2] * m4.m[3][1] - m4.m[0][1] * m4.m[2][2] * m4.m[3][3] - m4.m[0][2] * m4.m[2][3] * m4.m[3][1] - m4.m[0][3] * m4.m[2][1] * m4.m[3][2];
    rMat4.m[0][2] = m4.m[0][1] * m4.m[1][2] * m4.m[3][3] + m4.m[0][2] * m4.m[1][3] * m4.m[3][1] + m4.m[0][3] * m4.m[1][1] * m4.m[3][2] - m4.m[0][1] * m4.m[1][3] * m4.m[3][2] - m4.m[0][2] * m4.m[1][1] * m4.m[3][3] - m4.m[0][3] * m4.m[1][2] * m4.m[3][1];
    rMat4.m[0][3] = m4.m[0][1] * m4.m[1][3] * m4.m[2][2] + m4.m[0][2] * m4.m[1][1] * m4.m[2][3] + m4.m[0][3] * m4.m[1][2] * m4.m[2][1] - m4.m[0][1] * m4.m[1][2] * m4.m[2][3] - m4.m[0][2] * m4.m[1][3] * m4.m[2][1] - m4.m[0][3] * m4.m[1][1] * m4.m[2][2];
    
    rMat4.m[1][0] = m4.m[1][0] * m4.m[2][3] * m4.m[3][2] + m4.m[1][2] * m4.m[2][0] * m4.m[3][3] + m4.m[1][3] * m4.m[2][2] * m4.m[3][0] - m4.m[1][0] * m4.m[2][2] * m4.m[3][3] - m4.m[1][2] * m4.m[2][3] * m4.m[3][0] - m4.m[1][3] * m4.m[2][0] * m4.m[3][2];
    rMat4.m[1][1] = m4.m[0][0] * m4.m[2][2] * m4.m[3][3] + m4.m[0][2] * m4.m[2][3] * m4.m[3][0] + m4.m[0][3] * m4.m[2][0] * m4.m[3][2] - m4.m[0][0] * m4.m[2][3] * m4.m[3][2] - m4.m[0][2] * m4.m[2][0] * m4.m[3][3] - m4.m[0][3] * m4.m[2][2] * m4.m[3][0];
    rMat4.m[1][2] = m4.m[0][0] * m4.m[1][3] * m4.m[3][2] + m4.m[0][2] * m4.m[1][0] * m4.m[3][3] + m4.m[0][3] * m4.m[1][2] * m4.m[3][0] - m4.m[0][0] * m4.m[1][2] * m4.m[3][3] - m4.m[0][2] * m4.m[1][3] * m4.m[3][0] - m4.m[0][3] * m4.m[1][0] * m4.m[3][2];
    rMat4.m[1][3] = m4.m[0][0] * m4.m[1][2] * m4.m[2][3] + m4.m[0][2] * m4.m[1][3] * m4.m[2][0] + m4.m[0][3] * m4.m[1][0] * m4.m[2][2] - m4.m[0][0] * m4.m[1][3] * m4.m[2][2] - m4.m[0][2] * m4.m[1][0] * m4.m[2][3] - m4.m[0][3] * m4.m[1][2] * m4.m[2][0];
    
    rMat4.m[2][0] = m4.m[1][0] * m4.m[2][1] * m4.m[3][3] + m4.m[1][1] * m4.m[2][3] * m4.m[3][0] + m4.m[1][3] * m4.m[2][0] * m4.m[3][1] - m4.m[1][0] * m4.m[2][3] * m4.m[3][1] - m4.m[1][1] * m4.m[2][0] * m4.m[3][3] - m4.m[1][3] * m4.m[2][1] * m4.m[3][0];
    rMat4.m[2][1] = m4.m[0][0] * m4.m[2][3] * m4.m[3][1] + m4.m[0][1] * m4.m[2][0] * m4.m[3][3] + m4.m[0][3] * m4.m[2][1] * m4.m[3][0] - m4.m[0][0] * m4.m[2][1] * m4.m[3][3] - m4.m[0][1] * m4.m[2][3] * m4.m[3][0] - m4.m[0][3] * m4.m[2][0] * m4.m[3][1];
    rMat4.m[2][2] = m4.m[0][0] * m4.m[1][1] * m4.m[3][3] + m4.m[0][1] * m4.m[1][3] * m4.m[3][0] + m4.m[0][3] * m4.m[1][0] * m4.m[3][1] - m4.m[0][0] * m4.m[1][3] * m4.m[3][1] - m4.m[0][1] * m4.m[1][0] * m4.m[3][3] - m4.m[0][3] * m4.m[1][1] * m4.m[3][0];
    rMat4.m[2][3] = m4.m[0][0] * m4.m[1][3] * m4.m[2][1] + m4.m[0][1] * m4.m[1][0] * m4.m[2][3] + m4.m[0][3] * m4.m[1][1] * m4.m[2][0] - m4.m[0][0] * m4.m[1][1] * m4.m[2][3] - m4.m[0][1] * m4.m[1][3] * m4.m[2][0] - m4.m[0][3] * m4.m[1][0] * m4.m[2][1];
    
    rMat4.m[3][0] = m4.m[1][0] * m4.m[2][2] * m4.m[3][1] + m4.m[1][1] * m4.m[2][0] * m4.m[3][2] + m4.m[1][2] * m4.m[2][1] * m4.m[3][0] - m4.m[1][0] * m4.m[2][1] * m4.m[3][2] - m4.m[1][1] * m4.m[2][2] * m4.m[3][0] - m4.m[1][2] * m4.m[2][0] * m4.m[3][1];
    rMat4.m[3][1] = m4.m[0][0] * m4.m[2][1] * m4.m[3][2] + m4.m[0][1] * m4.m[2][2] * m4.m[3][0] + m4.m[0][2] * m4.m[2][0] * m4.m[3][1] - m4.m[0][0] * m4.m[2][2] * m4.m[3][1] - m4.m[0][1] * m4.m[2][0] * m4.m[3][2] - m4.m[0][2] * m4.m[2][1] * m4.m[3][0];
    rMat4.m[3][2] = m4.m[0][0] * m4.m[1][2] * m4.m[3][1] + m4.m[0][1] * m4.m[1][0] * m4.m[3][2] + m4.m[0][2] * m4.m[1][1] * m4.m[3][0] - m4.m[0][0] * m4.m[1][1] * m4.m[3][2] - m4.m[0][1] * m4.m[1][2] * m4.m[3][0] - m4.m[0][2] * m4.m[1][0] * m4.m[3][1];
    rMat4.m[3][3] = m4.m[0][0] * m4.m[1][1] * m4.m[2][2] + m4.m[0][1] * m4.m[1][2] * m4.m[2][0] + m4.m[0][2] * m4.m[1][0] * m4.m[2][1] - m4.m[0][0] * m4.m[1][2] * m4.m[2][1] - m4.m[0][1] * m4.m[1][0] * m4.m[2][2] - m4.m[0][2] * m4.m[1][1] * m4.m[2][0];
    
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
    for(int i = 0; i < 3; i++){
        rMat4.m[0][i] = u[i];
        rMat4.m[1][i] = v[i];
        rMat4.m[2][i] = w[i];
    }
    rMat4.m[0][3] = v3d::dot(u, -eye);
    rMat4.m[1][3] = v3d::dot(v, -eye);
    rMat4.m[2][3] = v3d::dot(w, -eye);
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
