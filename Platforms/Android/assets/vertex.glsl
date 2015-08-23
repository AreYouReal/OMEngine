#version 300 es

uniform mediump mat4 modelViewM;
uniform mediump mat4 projectionM;
uniform mediump mat4 normalM;

uniform mediump vec3 lightPos;

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;


//out mediump vec3 normal;
out lowp    vec3 lightColor;


void main(){
    mediump vec3 position =  vec3(modelViewM * vec4(vPosition, 1.0));
    mediump vec3 normal = normalize(vec3(normalM * vec4(vNormal, 1.0))); //(vNormal + 1.0) * 0.5;
    
    mediump vec3 lightDirection = normalize(lightPos - position);
    
    lowp float ndotl = max(dot(normal, lightDirection), 0.0);
    lightColor = ndotl * vec3(1.0, 1.0, 1.0); // White color - hard code for now
    gl_Position = projectionM * vec4(position, 1.0);
}