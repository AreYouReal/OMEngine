#version 300 es
uniform mediump mat4 MVPMatrix;

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;


out mediump vec3 normal;

void main(){
    gl_Position =  MVPMatrix * vec4(vPosition, 1.0);
    normal = (vNormal + 1.0) * 0.5;
}