#version 300 es

uniform mediump mat4 modelViewM;
uniform mediump mat4 projectionM;
uniform mediump mat4 normalM;

uniform mediump vec3 lightPos;

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vNormal;

out lowp    vec3 lightColor;

void main(){
    mediump vec3 position =  vec3(modelViewM * vPosition);
    mediump vec3 normal = normalize(vec3(normalM * vNormal));
    mediump vec3 lightDirection = normalize(lightPos - position);
    
    lowp float ndotl = max(dot(normal, lightDirection), 0.0);
    lightColor = ndotl * vec3(1.0, 1.0, 1.0); // White color - hard code for now
    gl_Position = projectionM * vec4(position, 1.0);
}