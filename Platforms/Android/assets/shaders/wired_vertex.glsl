#version 300 es

uniform mediump mat4 uModelViewM;
uniform mediump mat4 uProjectionM;
uniform lowp    vec3 uColor;

layout(location = 0) in vec4 aPosition;

out vec3 color;

void main(){
    gl_Position = uProjectionM * uModelViewM * aPosition;
    color = uColor;
}