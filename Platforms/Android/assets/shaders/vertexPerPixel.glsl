#version 300 es

uniform mediump mat4 uModelViewM;
uniform mediump mat4 uProjectionM;
uniform mediump mat4 uNormalM;

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTexCoord;

out mediump vec3 texCoord;
out mediump vec3 position;
out mediump vec3 normal;

void main(){
    position =  vec3(uModelViewM * aPosition);
    normal = normalize(vec3(uNormalM * vec4(aNormal, 1.0)));
    
    gl_Position = uProjectionM * vec4(position, 1.0);
    
    texCoord = aTexCoord;
}