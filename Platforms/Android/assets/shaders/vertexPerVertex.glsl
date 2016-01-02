#version 300 es


struct Light{
    lowp vec3 position;
    lowp vec4 color;
};

struct Material{
    lowp vec4 ambient;
    lowp vec4 diffuse;
    lowp vec4 specular;
    mediump float shininess;
};

uniform mediump mat4    uModelViewM;
uniform mediump mat4    uProjectionM;
uniform mediump mat4    uNormalM;
uniform         Light   uLight;

uniform Material uMaterial;
uniform lowp    float   uDissolve;

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTexCoord;

out mediump vec2 texCoord;
out lowp    vec4 ambientColor;
out lowp    vec4 diffuseColor;
out lowp    vec4 specularColor;

void main(){
    lowp vec3 normal = normalize(uNormalM * vec4(aNormal, 1.0)).xyz;
    mediump vec3 position = (uModelViewM * aPosition).xyz;
    lowp vec3 L = normalize(uLight.position - position);

    gl_Position = uProjectionM * uModelViewM * aPosition;
    
    float intensity = max(dot(normal, L), 0.0);
    if(intensity > 0.0){
        mediump vec3 E = normalize(-position);
        mediump vec3 R = normalize(-reflect(L, normal));
        
        diffuseColor = uMaterial.diffuse * uLight.color * intensity;
        specularColor = uMaterial.specular * uLight.color * pow(max(dot(R,E), 0.0), uMaterial.shininess );
    }else{
        diffuseColor = vec4(0.0f, 0.0f, .0f, 1.0);
        specularColor = vec4(0.0f, 0.0f, .0f, 1.0);
    }
    diffuseColor.a = 1.0;
    texCoord = aTexCoord.xy;
    ambientColor = uMaterial.ambient;
}