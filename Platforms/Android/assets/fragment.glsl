#version 300 es
precision mediump float;
out vec4 fragColor;

in mediump vec3 normal;
void main(){ fragColor = vec4(normal, 1.0); }