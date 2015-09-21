#version 300 es

uniform mediump mat4 uModelViewM;
uniform mediump mat4 uProjectionM;
uniform mediump mat4 uNormalM;
uniform mediump vec3 uLightPos;

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec4 aNormal;

out lowp vec4 color;

void main(){
    
    vec3 position = (uModelViewM * aPosition).xyz;
    vec3 L = normalize(uLightPos.xyz - position);
    
    vec3 normal = (uNormalM * aNormal).xyz;
    
    float intensity = max(dot(normal.xyz, L), 0.0);
    if(intensity > 0.0){
        color = vec4(.5, .5, .5, 1.0) * intensity;
    }
    
    gl_Position = uProjectionM * vec4(position, 1.0);
}