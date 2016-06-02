#version 300 es

struct Light{
    int type;
    lowp vec3 position;
    lowp vec4 color;
};

uniform mediump mat4    uModelViewM;
uniform mediump mat4    uProjectionM;
uniform mediump mat4    uNormalM;
uniform Light           uLight;
uniform lowp    vec4    uColor;

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec4 aNormal;

out lowp vec4 color;



void main(){
    
    vec3 position = (uModelViewM * aPosition).xyz;
    vec3 L;
    if(uLight.type == 0){
        L = normalize(uLight.position);
    }else{
        L = normalize(uLight.position - position);
    }

    vec3 normal = (uNormalM * aNormal).xyz;
    
    float intensity = max(dot(normal.xyz, L), 0.0);
    if(intensity > 0.0){
        color = uColor * intensity;
        
    }else{
        color = vec4(0.0, 0.0, 0.0, 1.0);
    }
    
    gl_Position = uProjectionM * vec4(position, 1.0);
}