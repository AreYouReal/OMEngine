#version 300 es

uniform mediump mat4 uModelViewM;
uniform mediump mat4 uProjectionM;

layout(location = 0) in vec4 aPosition;

out vec3 color;

void main(){
    gl_Position = uProjectionM * uModelViewM * aPosition;
    color = vec3(1, 0, 0);
}