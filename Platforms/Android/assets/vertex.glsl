//uniform mediump mat4 MODELVIEWPROJECTIONMATRIX;
//
//attribute mediump vec3 POSITION;
//attribute lowp vec3 NORMAL;
//
//varying lowp vec3 normal;
//
//void main(void){
//    normal = (NORMAL + 1.0) * 0.5;
//    gl_Position = MODELVIEWPROJECTIONMATRIX * vec4(POSITION, 1.0);
//}

#version 300 es
layout(location = 0) in vec4 vPosition;
void main(){ gl_Position = vPosition; }