#version 300 es
uniform mat4 uModelViewM;
uniform mat4 uProjectionM;
uniform mat4 uNormalM;

struct Light{
    lowp vec4 position;
    lowp vec4 color;
};

uniform Light uLight;

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTexCoord;
layout(location = 3) in vec3 aTangent;

out vec3 texCoord;
out vec3 position;
out vec3 normal;
out vec3 lightDirectionTS;

void main(){
    vec3 tmp;
    lowp vec3 normal    = vec3(uNormalM * vec4(aNormal, 1.0));
    lowp vec3 tangent   = vec3(uNormalM * vec4(aTangent, 1.0));
    lowp vec3 binormal  = cross(normal, tangent);
    
    position = vec3(uModelViewM * aPosition);
    gl_Position = uProjectionM * vec4(position, 1.0);
    
    // Directional light
    lowp vec3 lightDirectionES = normalize(uLight.position.xyz);
    
    lightDirectionTS.x = dot(lightDirectionES, tangent);
    lightDirectionTS.y = dot(lightDirectionES, binormal);
    lightDirectionTS.z = dot(lightDirectionES, normal);
    
    tmp.x = dot(position, tangent);
    tmp.y = dot(position, binormal);
    tmp.z = dot(position, normal);
    position = -normalize(tmp);
    
    texCoord = aTexCoord;
}