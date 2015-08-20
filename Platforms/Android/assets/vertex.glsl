#version 300 es
uniform mediump mat4 MVPMatrix;

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec3 vNormal;


out mediump vec3 normal;

void main(){
    gl_Position =  MVPMatrix * vPosition;
    normal = vNormal;
}