#version 300 es

uniform mediump mat4 uModelViewM;
uniform mediump mat4 uProjectionM;

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec4 aNormal;

out lowp vec4 normal;

void main(){
    normal = aNormal;
    gl_Position = uProjectionM * uModelViewM * aPosition;
}